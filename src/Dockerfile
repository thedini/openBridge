
# Needs to ensure that when the docker container is ran that
# it is ran with the usb ports connected with a specific name

# set base image (host OS)
FROM python:3.8

# set the working directory in the container
WORKDIR /code

# copy the dependencies file to the working directory
COPY PYTHON/requirements.txt .

# install dependencies
RUN pip install -r requirements.txt

# copy the content of the local src directory to the working directory
COPY PYTHON/ .

# command to run on container start
### CHANGE THE USB_PORT PARAM
CMD [ "python", "./csv_serial.py", "USB_PORT" ]