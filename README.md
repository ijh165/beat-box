# Beat-Box

Embedded app that plays drum beats on the BeagleBone Green.
- C app to talk to hardware
- Node.js + socket.io webserver for GUI
- UDP between C app and webserver

# Troubleshooting

1. If you get "use of const in strict mode" error when running the node server, 
upgrade your node in the bbg by the commands below (can also use higher version)

curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -
sudo apt-get install -y nodejs``<br/>

2. If you get "Error: ENOENT: no such file or directory, uv_cwd" error when 
running the node server, clean the npm cache by running the commands below in
another folder and then try running it again.

sudo npm cache clean -f

# References

1. favicon.ico taken from here http://findicons.com/icon/421694/music