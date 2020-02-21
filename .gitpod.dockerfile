FROM gitpod/workspace-full

RUN sudo apt-get update \
 && sudo apt-get install -y \
    libsdl2-2.0 \
 && sudo rm -rf /var/lib/apt/lists/*
