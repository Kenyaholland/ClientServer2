# COP 4635 Project 1B

![Points badge](../../blob/badges/.github/badges/points.svg)

Your hacker friends received your message, congratualtions! They noticed, that there is a strange webserver of an opposing hacker teams website. After further investigation it seems like they are hiding a message. Unfortunatly, there are many links you have to follow to find the hidden message.

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

following the link will lead you to a new site, which again leads you to a new side, and so on. In total this may happen up to 100000 times. The last page will contain no link, but a message.

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

Your program should then print exactly the message. It is save to assume that `pageID` is exactly 100 characters long.

### Using the testserver

To try out your program before finally hacking the opposing teams server, one of your friends developed a test server. There are two ways for running it:

- Install [Go](https://golang.org/), a great language for smaller networking related projects.
- Install [Docker](https://www.docker.com/), a very important tool for programming in general. It allows you to easily create and manage virtual machines.

### Implementation details

- A typical GET request the the client sends should look like this

    ```console
    GET /newLink HTTP/1.1
    
    ```
