FROM gitpod/workspace-full-vnc

RUN sudo apt-get update \
 && sudo apt-get install -y \
    libsdl2-dev \
    neofetch \
    libncurses5-dev libncursesw5-dev \
 && sudo rm -rf /var/lib/apt/lists/*
