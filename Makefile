NAME = scop

IMAGE_NAME=fedora-container
CONTAINER_NAME=fedora-running

all: build run
	podman exec -it $(CONTAINER_NAME) /bin/bash \
		-c "cd ./project && make re && exec /bin/bash"

$(NAME): all

build:
	podman build -t $(IMAGE_NAME) .

run:
	xhost +local: || true
	podman run -dit \
		--name $(CONTAINER_NAME) \
		--security-opt label=disable \
		-e DISPLAY=$$DISPLAY \
		-e XDG_RUNTIME_DIR=/tmp \
		-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
		-v $(PWD):/workspace:Z \
		-w /workspace \
		--device /dev/dri \
		$(IMAGE_NAME) || true

stop:
	podman kill $(CONTAINER_NAME) || true

start:
	podman start $(CONTAINER_NAME) || true

rmc:
	podman rm -f $(CONTAINER_NAME) ||true

clean: stop rmc

fclean: clean
	podman rmi -f $(IMAGE_NAME) || true

re: fclean all

.PHONY: all build run stop start rmc end clean fclean re