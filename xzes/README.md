# XZES Projects

These components include:

- `transformer`
- `cgi-glue`
- `frontend`

Each project includes it's own README which self-describes the component, what it does, and how it works.

## xzes.conf

This file is the Apache config for the XZES40-Transformer application.
Feel free to edit this to change the port the application runs on, add security certificates, pretty much anything you would do in an Apache HTTPd config.

## xzesd.service

This is the systemd service file for xzesd.
It automatically restarts the application, manages forking the process into the background, and aggregates xzesd stdout to the Linux logging service.
