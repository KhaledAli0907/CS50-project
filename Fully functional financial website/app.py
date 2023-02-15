import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
from datetime import datetime

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    result = db.execute(
        "SELECT symbol, name, SUM(shares) AS totalshares, price FROM purchase WHERE user_id = ? GROUP BY symbol", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]['cash']
    total = cash

    for row in result:
        total += row["price"] * row["totalshares"]

    return render_template("index.html", rows = result, cash = usd(cash), usd = usd, total=usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        result = lookup(request.form.get("symbol").upper())
        # validate the input
        if not result:
            return apology("Symbol does not exist!")
        # get price user_id cash
        symbol = result["symbol"]
        user_id = session["user_id"]
        price = result["price"]
        plainshares = request.form.get("shares")
        if not plainshares.isnumeric() or int(plainshares) < 0:
            return apology("Invalid Number of shares")
        shares = int(plainshares)
        name = result["name"]
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]['cash']
        change = float(cash - price*shares)
        if change < 0:
            return apology("Not Eough Cash in your wallet")
        else:
            # update the user's cash to be the remainder after purchase
            db.execute("UPDATE users SET cash = ? WHERE id = ?", change, user_id)
            # Insert into purchase table
            db.execute("INSERT INTO purchase (user_id, symbol, name, shares, price, time_stamp, type) VALUES (?, ?, ?, ?, ?, ?, ?)", \
                       user_id, symbol, name, shares, price, datetime.now(), 'Purchase')
            # Insert into own table with the shares the user bought
            db.execute("INSERT INTO own (user_id, symbol, name, shares) VALUES (?, ?, ?, ?)", user_id, symbol, name, shares)

            flash("Bought")

    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    table = db.execute("SELECT * FROM purchase WHERE user_id = ?", user_id)
    return render_template("history.html", table=table, usd=usd)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # if get display search quote html
    if request.method == "GET":
        return render_template("quote.html")
    # if POST, render quote search result
    else:
        # Get the symbol from the user
        symbol = request.form.get("symbol")
        # if left blank
        if not symbol:
            return apology("Must provide a symbol!")
        # use lockup function and store it
        result = lookup(symbol.upper())
        if not result:
            return apology("Symbol Does not exist")

        return render_template("result-quote.html", name = result["name"], price = usd(result["price"]), symbol = result["symbol"])



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # get the username and password from html
        name = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        # validate user input
        if name == "" or len(db.execute('SELECT username FROM users WHERE username = ?', name)) > 0:
            return apology("Invalid username = empty field or already exists")
        if password == "" or password != confirmation:
            return apology("Password Doesn't match")

        #insert username and password into the database
        db.execute('INSERT INTO users (username, hash)  VALUES(?, ?)', name, generate_password_hash(password))
        # quarry the database for username
        rows = db.execute('SELECT * FROM users WHERE username = ?', name)
        # login the user
        session["user_id"] = rows[0]["id"]
        # Set user's cash to 0
        db.execute("UPDATE users SET cash = ? where id = ?", 0, session["user_id"])
        # redirect user to the home page
        return redirect("/")

    # if users clicked GET event
    elif request.method == "GET":
        # redirect to register page
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # get the user_id, symbol and number of shares from database
    user_id = session["user_id"]
    if request.method == "GET":
        result = db.execute("SELECT symbol, name, shares FROM purchase WHERE user_id = ? GROUP BY symbol", user_id)
        return render_template("sell.html", rows=result)

    # if the method is post
    else:
        # get user's inputs
        inputShares = int(request.form.get("shares"))
        inputSymbol = request.form.get("symbol")
        sharesowned = int(db.execute("SELECT SUM(shares) as shares FROM purchase WHERE user_id = ? AND symbol = ?", user_id, inputSymbol)[0]["shares"])
        # validate user input
        if inputSymbol == None or inputSymbol == "":
            return apology("You Have To Select A Symbol")
        if sharesowned < inputShares or inputShares <= 0:
            return apology("shares to sell greater than owned shares or Invalid number")

        pricenow = lookup(inputSymbol)['price']
        namenow = lookup(inputSymbol)["name"]
        recash = pricenow * inputShares
        cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]['cash']
        # update users current cash to be cash plus the sell transaction
        db.execute("UPDATE users SET cash = ? WHERE id = ?", recash + cash, user_id)
        # update the user's owned stock
        db.execute("INSERT INTO purchase (user_id, symbol, shares, price, time_stamp, name, type) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   user_id, inputSymbol, -inputShares, pricenow, datetime.now(), namenow, 'Sell')
        # get the latest shares count
        # sharesnow = int(db.execute("SELECT SUM(shares) AS shares FROM purchase WHERE user_id = ? AND symbol = ?", user_id, inputSymbol)[0]["shares"])
        # delete the stock from database if user sold it all
        #if sharesnow <= sharesowned:
            #db.execute("DELETE FROM purchase WHERE user_id = ? AND symbol = ?", user_id, inputSymbol)

        flash("Sold")
        return redirect("/")


@app.route("/recharge", methods=["GET", "POST"])
@login_required
def recharge():
    """ Recharge User's balance """
    # get the user id
    user_id = session["user_id"]
    # if user clicked on page
    if request.method == "GET":
        # render recharge HTML
        return render_template("recharge.html")
    # else if user post
    else:
        # apply luhn's algorithm
        CreditCard = request.form.get("cridetcard")
        CreditCard = CreditCard.replace("-", "")
        CreditCard = CreditCard.replace(" ", "")
        # Hold the Card num. length
        y = len(CreditCard)
        # Copy the Card into list to modify
        copycard = CreditCard[::-1]
        # put try to handle value errors
        try:
            # apply the algorithm with list comprehension
            firstnumbers = [int(i) for i in copycard[0::2]]
            othernumbers = [float((int(x) * 2 // 10)) + float((int(x) * 2 % 10)) for x in copycard[1::2]]
            totalsum = sum(firstnumbers) + sum(othernumbers)
            # validate card number
            if totalsum % 10 == 0:
                if y in [13, 16] and CreditCard[0] == '4' or y == 15 and CreditCard[0:2] in ['34', '37'] or y == 16 and 51 <= int(CreditCard[0:2]) <= 55 :
                    # get the amount from HTML
                    amount = request.form.get("amount")
                    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
                    # update users table with the new cash amount
                    db.execute("UPDATE users SET cash = ? WHERE id = ?", int(amount) + int(cash), user_id)
                    # return to the home page
                    flash("Successful Recharge")
                    return redirect("/")
                else:
                    return apology("Invalid number")

            else:
                return apology("Invalid cardnumber")

        except ValueError:
            flash("Invalid CardNumber")
            return render_template("recharge.html", invalid=True)

@app.route("/changepass", methods=["GET", "POST"])
@login_required
def changepass():
    """ Change user's Password """
    # get user's id
    user_id = session["user_id"]
    # when user click the template
    if request.method == "GET":
        return render_template("changepass.html")

    # when user post
    else:
        # quary database by user id
        rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)
        # get old password from user
        oldPassword = request.form.get("oldpassword")

        # validate it's correct
        if not check_password_hash(rows[0]["hash"], oldPassword):
            flash("Old password incorrect")
            return render_template("changepass.html", invalid=True)

        # get input from user
        newPassword = request.form.get("newpassword")
        confirmation = request.form.get("confirmation")

        # validate passwords match
        if newPassword != confirmation:
            flash("Passwords Doesn't Match!")
            return render_template("changepass.html", invalid=True)

        # Change the database with the new password
        db.execute("UPDATE users SET hash = ? WHERE id = ?",generate_password_hash(newPassword) ,user_id)

        # return the user to homepage
        flash("Password Changed")
        return redirect("/")


