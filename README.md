# WEBSERV
We'reworking on a project that involves writing our own HTTP server, which we'll be able to test using a real browser. HTTP is a protocol that is extensively used on the internet, and understanding its intricacies will be valuable, even if we're not directly involved in website development.

**Description:**
Webserv is a custom HTTP server developed in C++. This project aims to provide a comprehensive understanding of the HTTP protocol, while also demonstrating proficiency in C++ programming and teamwork.

The project allows users to run their own HTTP server, handle client requests, and deliver web pages to clients. It supports various HTTP methods, including GET, POST, and DELETE, and enables clients to upload files. The server ensures non-blocking operations and uses a single poll() (or equivalent) for all I/O operations.

**Features:**
- HTTP server implementation in C++ 98
- Supports multiple HTTP methods: GET, POST, DELETE
- Non-blocking server operations
- Single poll() for all I/O operations
- File upload capability
- Compatibility with popular web browsers
- Error handling and default error pages
- Configuration file support
- Team collaboration and effective teamwork

**Getting Started:**
1. Clone the repository: `git clone git@github.com:elhayyany/WEBSERV.git`
2. Navigate to the project directory: `cd webserv`
3. Compile the source files: `make`
4. Run the Webserv executable: `./webserv [configuration file]`

Make sure to replace `[configuration file]` with the path to your desired configuration file. If no configuration file is provided, the server will use the default settings.

**Authors:**
- AYOUB AITBELHAJ ([@aaitbelh](https://github.com/aaitbelh))
- Mohamed Amellal ([@Mohamed-Amellal](https://github.com/Mohamed-Amellal))
- ABDELAZIZ ELHAYYANY ([@elhayyany](https://github.com/elhayyany))

**License:**
This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute it according to the terms of the license.

**Resources:**
- [HTTP Protocol Specification](https://www.rfc-editor.org/rfc/rfc2616.txt)
- [NGINX Official Website](https://nginx.org/)
- [Hands-On Network Programming with C](https://www.amazon.com/Hands-Network-Programming-programming-optimized/dp/1789349869)

