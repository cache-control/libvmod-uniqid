# libvmod-uniqid
Varnish module - Generate unique request ID.

## Introduction
This module utilizes CityHash to generate unique request IDs. A key use
case is during postmortem analysis, where these IDs can help trace the
lifecycle of a request as it propagates through, potentially, a
hierarchy of caches.

The C++ library source is available from the repo below. I converted it
to C for use in this project.

```
https://github.com/google/cityhash
```

## Requirements
autotools and Varnish header files are required for building.

```sh
sudo apt install build-essential libvarnishapi-dev libvarnishapi3 varnish
```

## Build

```
./autogen.sh
./configure
make
```

## Usage
VCL
```
vcl 4.1;

import uniqid;

backend default {
    .host = "23.192.228.84";
    .host_header = "example.com";
    .port = "80";
}

sub vcl_recv {
    // downstream caches should not regenerate id
    if (! req.http.x-req-uuid) {
        set req.http.x-req-uuid = uniqid.get(req.url);
    }
}

sub vcl_backend_fetch {
    // use .host_header
    unset bereq.http.host;
}

sub vcl_deliver {
    // reflect back to client
    set resp.http.x-req-uuid = req.http.x-req-uuid;
}
```

Add the custom header to logs for postmortem lookup.
```
varnishncsa -a -F 'reqid:%{x-req-uuid}i> %U %s'
```

## Example
Each request, for the same asset, generates a unique ID.
```sh
$ curl -sI 0:6081'#[1-3]' | fgrep x-req-uuid
x-req-uuid: 598542634311601346017491590256066505003
x-req-uuid: 174675174631095603345053404077005614714
x-req-uuid: 84666192971502837633702765502394338531
```
