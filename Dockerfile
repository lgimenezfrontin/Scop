FROM fedora:latest

RUN dnf -y update && \
    dnf -y install \
        gcc gcc-c++ make \
        glfw glfw-devel \
        glew glew-devel \
        mesa-libGL mesa-libGL-devel \
        mesa-libGLU mesa-libGLU-devel \
        libX11 libX11-devel \
        libXrandr libXrandr-devel \
        libXinerama libXinerama-devel \
        libXcursor libXcursor-devel \
        libXi libXi-devel && \
    dnf clean all

CMD ["/bin/bash"]