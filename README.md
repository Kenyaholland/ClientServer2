[![Work in Repl.it](https://classroom.github.com/assets/work-in-replit-14baed9a392b3a25080506f3b7b6d57f295ec2978f6f33ec97e36a161684cbe9.svg)](https://classroom.github.com/online_ide?assignment_repo_id=4059176&assignment_repo_type=AssignmentRepo)
# COP 4635 Project 1B

![Points badge](../../blob/badges/.github/badges/points.svg)

Your hacker friends received your message, congratulations! They noticed that there is a [strange webserver of an opposing hacker team](http://powerful-plains-35491.herokuapp.com). After further investigation, it seems like they are hiding a message. Unfortunately, there are many links you have to follow to find the hidden message.

## The task

You are supposed to write a program, again only using native system calls, which is called like this

    src/hackServer <hostname> <port>

The program is supposed to perform a GET request to `http://hostname:port/`. The response will look like this

    ```html
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8"/>
        <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
        <title>Highly sensitive data</title>
    </head>
    <body>
        <a href="/pageID">Number</a>
    </body>
    </html>
    ```

Following the link will lead you to a new site, which again leads you to a new side, and so on. In total, this may happen up to 100000 times. The last page will contain no link but a message.

    ```html
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8"/>
        <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
        <title>Highly sensitive data</title>
    </head>
    <body>
        <p>message</p>
    </body>
    </html>
    ```

Your program should then print exactly the message. It is safe to assume that `pageID` is exactly 100 characters long.

### Using the local test server

To try out your program before finally hacking the opposing teams' server, one of your friends developed a test server. There are three ways for running it:

- Use the black button on top, which will lead you to repl.it.

- Install [Go](https://golang.org/). The test server can be launched using

    ```console
    go run .github/testServer.go 8080 10 "This is the message"
    ```

- Install [Docker](https://www.docker.com/). After installation, the development server can be run with

    ```console
    docker run --rm -it -p 8080:8080 -v "$PWD":/go golang go run .github/testServer.go 8080 10 "This is the message"
    ```

Both commands will launch the server at [http://localhost:8080/](http://localhost:8080/). You have to follow 10 links, and then "This is the message" will be shown (you can also use different arguments). Please do not use [http://powerful-plains-35491.herokuapp.com](http://powerful-plains-35491.herokuapp.com) for development.

### Implementation details

The exact implementation is up to you. The only condition is that you use linux socket programming. This means that the functions for networking should be `socket`, `gethostbyname`, `connect`, `recv`, `send`, and so on. You are completely free on how to deal with the rest, e. g. using c or c++ strings, determining the address, etc.

- Please do not use [http://powerful-plains-35491.herokuapp.com](http://powerful-plains-35491.herokuapp.com) for development, as we may run into usage limits. Test your code with your local test server, and if you are sure that it works, you can try it out once on the [http://powerful-plains-35491.herokuapp.com](http://powerful-plains-35491.herokuapp.com) link.
- Do not change anything in the .github folder. Doing so will be considered attempted deception. If you change or delete it by accident, ask the teacher or one of the TAs for help.
- Have a look at the examples [examples/tcpClient3-1.c](examples/tcpClient3-1.c) and [examples/tcpServer3-1.c](examples/tcpServer3-1.c). Your implementation should follow these and use no additional libraries.
- Put your code into [src/main.cpp](src/main.cpp). It should run with the code in [run.bash](run.bash).

    ```console
    bash run.bash
    ```

    Test your program using the local test server.
- A typical GET request the client sends should look like this

    ```console
    GET /newLink HTTP/1.1
    Host: hostname
    
    ```

    `hostname` should be the actual name of the host.

- The newlines in the GET requests and responses are important and should be done using `\r\n`.

- Do not send zero bytes over the internet connection, as this may result in strange behavior.

### Submission process

Your project is graded using Github. We do not accept any other submission method.

### Grading

You will get 90 points if the test cases succeed and your code is implemented as intended. The remaining 10 points will be given manually by the TA and judge your coding style. A good coding style means:

- Useful variable and function names
- Comments to explain what the code does
- Proper indentation

In total, this will add up to 100 points.
