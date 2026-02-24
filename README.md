# PING
`ft_ping` is a minimal `ping` network utility written in C. It uses ICMP Echo requests to a target **IPv4 address** and prints reachability and roundtrip time.

## Current Features
- Sending ICMP Echo request
- Receiving ICMP reply and error messages,
- Accepts IPv4 formated adress asan  argument
- Accepts --ttl=N argument which specifies time to live in IP header. Will report if the target is not reachable\
- Makefile
- Nix-shell to test it agains the refference

## Build
```sh
make
```
This produces executable:
```sh
./ft_ping
```

## Usage
```sh
sudo ./ft_ping <ipv4-adress> --ttl=<n>
```
Examples:

```sh
./ft_ping 8.8.8.8
```
```sh
sudo ./ft_ping 8.8.8.8 --ttl=10
```
## Coming in the future
- Accepting domains as arguments
- -v flag for verbose output
- spesific error messages based on ICMP response










 
