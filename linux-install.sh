#/bin/bash

echo ' LanGong LGame QMineSweeper Install Script'

download () {
  echo ' >> Download'
  echo '    -> (0/2) Downloading source from GitHub'
  git clone https://github.com/langong-dev/QMineSweeper.git "$HOME/.qms-cache"
  echo '    -> (1/2) Loading source'
  cd "$HOME/.qms-cache"
  echo '    -> (2/2) Done!'
}

compile (){
  echo ' >> Compile'
  echo '    -> (0/2) Qt Making'
  qmake QMineSweeper.pro
  echo '    -> (1/2) C Making'
  make
  echo '    -> (2/2) Done!'
}

installto (){
  bin="$1"
  dir="$2"
  echo ' >> Install'
  echo '    -> (0/1) Install bin to "'$bin'"'
  cp QMineSweeper "$bin/qminesweeper" || cp bin/QMineSweeper "$bin/qminesweeper"
  cp -r ./* "$dir/qminesweeper"
  echo '    -> (1/1) Done!'
}

if [ $1 == 'dir' ]; then
  download
  compile
  installto "$2" "$3"
else
  download
  compile
  installto "/usr/bin" "/usr/share"
fi
