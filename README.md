<div align="center">
    <h1>POP3 Client (29) </h1>
    <h3>fischer_project_2</h3>
    <h5>David Fischer, 5CHIF (03)</h5>
</div>

<div align="center">

[![License](https://img.shields.io/badge/License-Boost%201.0-blue.svg)](https://www.boost.org/LICENSE_1_0.txt)
[![time tracker](https://wakatime.com/badge/github/konstfish/fischer_project_2.svg)](https://wakatime.com/badge/github/konstfish/fischer_project_2)
[![CodeFactor](https://www.codefactor.io/repository/github/konstfish/fischer_project_2/badge?s=c397d4ac5a23901eaf68a5ace37595352a91e3d1)](https://www.codefactor.io/repository/github/konstfish/fischer_project_2)

</div>

## 💦 Short Summary
The goal of this assignment was the creation of a POP3 Client, including support for GnuTLS. The project was implemented using Protobuf, as well as gRPC.

```
  _           _     _     _        0  
  | |__  _   _| |__ | |__ | | ___    o   
  | '_ \| | | | '_ \| '_ \| |/ _ \    O 
  | |_) | |_| | |_) | |_) | |  __/   0  
  |_.__/ \__,_|_.__/|_.__/|_|\___|    o         
```

## 📈 Usage

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
  -i,--interactive            Enable Interactive Shell
```

## 📝 Requirements

* [Protobuf](https://developers.google.com/protocol-buffers)
* [gRPC](https://grpc.io/)

- - -

* [CLI11](https://github.com/CLIUtils/CLI11)
* [JSON for Modern C++](https://github.com/nlohmann/json)
* [spdlog](https://github.com/gabime/spdlog)
* [tabulate](https://github.com/p-ranav/tabulate)
* [httplib](https://github.com/yhirose/cpp-httplib)
* [inja](https://github.com/pantor/inja)

## 🔨 Compile

`mkdir -p build && cd build && meson .. && ninja`

## 🖥️ Example Interactive Client
```
fischer_project_2/build on  master [!] at ☸️  docker-desktop took 23s
➜ ./bubble -j ../config.json -i
[2021-03-14 20:24:36.812] [console] [info] Connection successfully established
[2021-03-14 20:24:37.723] [console] [info] Successfully Logged in!
[2021-03-14 20:24:37.725] [console] [info] gRPC Server listening on 0.0.0.0:50051
bubble> help
Bubble Interactive Client:
dl / download <message_id>  - download email with id
rm / delete <message_id>    - delete email with id
ls / list <amount>          - list amount of mails
exit                        - disconnects the session
help                        - displays this message
bubble> ls 5
+------------+-----------------------------------+------------------------------------------------------------------+---------------------------------+
| Message ID | Recieved From                     | Subject                                                          | Date                            |
+------------+-----------------------------------+------------------------------------------------------------------+---------------------------------+
| 6          | David Fischer <david@konst.fish>  | Fwd: Philip Trauner und andere teilen ihre Meinung auf LinkedIn  | Sun, 7 Mar 2021 21:13:56 +0100  |
+------------+-----------------------------------+------------------------------------------------------------------+---------------------------------+
| 5          | David Fischer <david@konst.fish>  | Fwd: Sie werden wahrgenommen, David Fischer                      | Sun, 7 Mar 2021 21:13:53 +0100  |
+------------+-----------------------------------+------------------------------------------------------------------+---------------------------------+
| 4          | David Fischer <david@konst.fish>  | Fwd: DJI MSDK iOS Update Delay Notice                            | Sun, 7 Mar 2021 21:13:46 +0100  |
+------------+-----------------------------------+------------------------------------------------------------------+---------------------------------+
| 3          | David Fischer <david@konst.fish>  | Fwd: Hot in March                                                | Sun, 7 Mar 2021 21:13:13 +0100  |
+------------+-----------------------------------+------------------------------------------------------------------+---------------------------------+
| 2          | David Fischer <david@konst.fish>  | Fwd: WIRED und andere teilen ihre Meinung auf LinkedIn           | Sat, 6 Mar 2021 22:59:52 +0100  |
+------------+-----------------------------------+------------------------------------------------------------------+---------------------------------+
bubble> dl 6
[2021-03-14 20:24:47.324] [console] [info] Saved message with ID: 6
bubble> rm 6
[2021-03-14 20:24:51.523] [console] [info] Deleted message with ID: 6
bubble> exit
[2021-03-14 20:24:54.536] [console] [info] Successfully Quit Session!
```

## 🌐 Web Interface

![web interface](/doc/images/web_interface.png?raw=true)

