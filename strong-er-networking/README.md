# strong-er-networking
Networking library shared between Strong ER and Integration

# Setup
To use the library within other QT apps, run the subtree command within the app's directory

`git subtree add  --prefix strong-er-networking https://github.com/Strong-Room/strong-er-networking.git main --squash`

Then within the app's .pro file, add the include line for the library

`include(strong-er-networking/strong-er-networking.pri)`

# Usage
To start sending and receiving messages use the NetworkManager class's newInstance function to instantiate a NetworkManager instance.

## Sending messages
NetworkManager's sendMessage can then be used to send messages to the network, with a connection type (declared in connection.h under Datatype) and
a QString message. This message is then passed to any connections on the network.

## Receiving messages
NetworkManager's newMessage can be used to receive messages from the network. A connection type and QString message will be received.

# Update library within app repo
To update subtree within containing repo:
`git subtree pull --prefix strong-er-networking https://github.com/Strong-Room/strong-er-networking.git main --squash`

Ensure when making changes within this repo to run the subtree pull command in each repo containing it
