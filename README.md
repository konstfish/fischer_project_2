<div align="center">
    <h1>POP3 Client</h1>
    <h3>fischer_project_2</h3>
</div>

<div align="center">

[![License](https://img.shields.io/badge/License-Boost%201.0-blue.svg)](https://www.boost.org/LICENSE_1_0.txt)
[![time tracker](https://wakatime.com/badge/github/konstfish/fischer_project_2.svg)](https://wakatime.com/badge/github/konstfish/fischer_project_2)

</div>

## Short Summary
The goal of this assigment was the creation of a POP3 Client, including support for GnuTLS.

## Usage

```
POP3 Client
Usage: ./bubble [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -s,--server TEXT            Server Domain
  -u,--user TEXT              Account Name
  -p,--pass TEXT              Account Password
  -t,--tls                    Connect using TLS
  --port INT                  Specify custom port
  -j,--json TEXT              Relative path to JSON file containing the Config
  -d,--download INT           Download Email with specified ID
  -r,--remove INT             Delete Email with specified ID
  -l,--list INT:Range of Emails limited to sensible values
                              List
  -i,--interactive            Enable Interactive Shell
```

## Compile

`mkdir build && cd build && meson .. && ninja`
