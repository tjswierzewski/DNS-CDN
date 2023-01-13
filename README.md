# CDN Network

A C++ project that deploys a CDN network for a Wikipedia clone. Makes one DNS server instance and a number of HTTP servers based on a internal file. Scamper is used on all servers to measure ping to clients. No third party libraries are used in this project.

---
## Usage:
### Build:
For project submission all files should already be built. If there is a problem or you would like to rebuild the project based on new servers use the following command
`./buildCDN -p <port> -o <origin> -n <name> -u <username> -i <keyfile>`
### Deploy
`./deployCDN -p <port> -o <origin> -n <name> -u <username> -i <keyfile>`
### Run
`./runCDN -p <port> -o <origin> -n <name> -u <username> -i <keyfile>`
### Stop
`./runCDN -p <port> -o <origin> -n <name> -u <username> -i <keyfile>`

The list of servers is found in `DNS/servers.txt`
The ranked page views for caching are found in `pageviews.csv`

---
## Design
### scamper
For active measurement of latency for each server to the client the measurement tool scamper us used. A host instance is created on the DNS server using `sc_remoted` and creates a TCP server. A instance of `scamper` is run on each HTTP server that connects to the DNS server and waits for commands. When a new client queries to DNS server it is added to a file called `connectedClients`. Every 15 seconds the DNS server invokes the `sc_attach` command using the UNIX socket for each HTTP server. The results are then returned on a pipe created when `sc_attach` is executed.

### DNS
The DNS server loads in a list of HTTP servers it can direct traffic to. Then a CSV containing geo-location data based on IPv4 ranges. Using this list the location of each server is added to the HTTP servers. Epoll is used to efficiently wait for a socket to be read from. When a UDP message is received it is parsed as DNS message. If the request is for the given `<name>` a DNS response is crafted. If this is the first query from a client, the location of the client is looked up with a binary search of location data. This is returned to the client with a short TTL. The short TTL allows for a scamper measurement to give connection information to the client. If the latency from a server is lower then the currently preferred server the preferred server is replaced and the TTL is reset. If a measurement from the preferred server is read, it is assumed that no other server was better and the TTL is doubled. 

### HTTP
The HTTP servers also use epoll but this is to handle requests from multiple hosts concurrently. When a request it received the path is checked against the files in the folder `cache`. If the path is not found then a request is sent to the origin which is forwarded to the client. For the cache only file caching is used. No ram caching is used on the HTTP servers so the memory usage should be well below 20MB. There is a bug when deployed where pages requested from the origin fail to send the whole message.


