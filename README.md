<h1 align="center" id="title">IRC Server Project</h1>
<p align="center"><img src="https://miro.medium.com/v2/resize:fit:2000/1*nQRvYcNqp7inbwt3m7xxlQ.jpeg" alt="project-image"></p>
<p id="description">This project involves creating an IRC (Internet Relay Chat) server in C++ 98. The server will handle multiple clients, support various IRC commands, and facilitate communication between clients via channels.</p>

<h2>🧐 Requirements</h2>

- Develop an IRC server in C++ 98.
- The server must handle multiple clients simultaneously without hanging.
- All I/O operations must be non-blocking.
- Use `poll()` or equivalent for handling all operations.
- Implement specific IRC commands: AUTHENTICATE, NICK, USER, JOIN, PRIVMSG, KICK, INVITE, TOPIC, MODE.
- Ensure robust error handling and testing.
- Use a reference IRC client for testing.



###  _ :crystal_ball: Definition


> "IRC (Internet Relay Chat) is a **protocol for real-time text messaging between internet-connected computers created in 1988**. It is mainly used for group discussion in chat rooms called “channels” although it supports private messages between two users, data transfer, and various server-side and client-side commands. As of April 2011, the top 100 IRC networks served over 500,000 users at a time on hundreds of thousands of channels." ([source](https://www.radware.com/security/ddos-knowledge-center/ddospedia/irc-internet-relay-chat/))

### _ :floppy_disk: Subject requirements


**1.** Code the IRC server in C++98
**2.** Your binary should look like this : `./ircserv <port> <password>`
**3.** You should use `poll()` and only **once**.
**4.** The client/server communication will be TCP/IP (v4 or v6)
**5.** The following features are mandatory :
> **i)** You should be able to register, i.e to define a nickname, a username

> **ii)** You should be able to join a channel, send or receive private messages

> **iii)** Every message sent by a client in a channel must be received by all of the clients who have joined this channel

> **iv)** You should have normal users and operators, and implement the operators' specific commands
**6.** No leaks (even still reachables) and handle the signals (partial messages with `CTRL+D`, suspended process with `CTRL+Z`, and obviously SIGINT (`CTRL+C`))
**7.** (bonuses) implement a bot + file transfer.


___
### :high_brightness: Commands that we implemented :



| Command | Description |
| :-----------: | :----------- |
| Invite | The `INVITE` command is used to invite a user to a channel. |
| Join | The `JOIN` command indicates that the client wants to join the given channel(s), each channel using the given key for it. |
| Kick | The `KICK` command can be used to request the forced removal of a user from a channel. |
| Kill | The `KILL` command is used to close the connection between a given client and the server they are connected to. `KILL` is a privileged command and is available only to IRC Operators. |
| Mode | The `MODE` command is used to set or remove options (or modes) from a given target. Our user modes : i, o. Our channels modes: b,k,m,o,p,s,t,v |
| Nick | The `NICK` command is used to give the client a nickname or change the previous one. |
| Pass | The `PASS` command is used to set a ‘connection password’. If set, the password must be set before any attempt to register the connection is made. |
| Privmsg | The `PRIVMSG` command is used to send private messages between users, as well as to send messages to channels. |
| Quit | The `QUIT` command is used to terminate a client’s connection to the server. The server acknowledges this by replying with an `ERROR` message and closing the connection to the client. |
| Topic | The `TOPIC` command is used to change or view the topic of the given channel. |
| User | The `USER` command is used at the beginning of a connection to specify the username and realname of a new user. |

## :gift: Some tips, graphs and other ressources


### _ :books: Helpful links


- **To understand what exactly is an IRC and begin the project** : [Chirc](http://chi.cs.uchicago.edu/chirc/irc.html) (An irc subject from a Chicago uni) and the next pages too.


- **Regarding the client/server connection** : [Beej's Guide to network programming](https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf). This is super super helpful to better understand what is a socket, what the `poll()` function entails, which system calls we should use or in which order, and so on.


- **A link to the IRC Client Protocol with a good formatting** : [Modern IRC Client Protocol](https://modern.ircdocs.horse/). You will find there a description of all the commands with correct syntax, expected Numerical Replies,...



<h2>👥 Contributors</h2>

- [Houmam Ait Salimane](https://profile.intra.42.fr/users/hait-sal)
- [Hamza Aarab](https://profile.intra.42.fr/users/haarab)
