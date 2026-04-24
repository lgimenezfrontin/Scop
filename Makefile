IMAGE_NAME=fedora-container
CONTAINER_NAME=fedora-running

.PHONY: build run make clean shell

make: build run shell

build:
	docker build -t $(IMAGE_NAME) .

run:
	xhost +local: || true
	docker run -dit \
		--name $(CONTAINER_NAME) \
		-e DISPLAY=$$DISPLAY \
		-e XDG_RUNTIME_DIR=/tmp \
		-v /tmp/.X11-unix:/tmp/.X11-unix \
		-v $(PWD):/workspace \
		-w /workspace \
		--device /dev/dri \
		$(IMAGE_NAME)

shell:
	docker exec -it $(CONTAINER_NAME) /bin/bash

stop:
	docker kill $(CONTAINER_NAME) || true

start:
	docker start $(CONTAINER_NAME) || true

rmc:
	docker rm -f $(CONTAINER_NAME) ||true

end: stop rmc

clean: end
	docker rmi -f $(IMAGE_NAME) || true

re: clean make