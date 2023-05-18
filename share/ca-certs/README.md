This directory holds the certificate bundles that we use on Windows and
optionally on other platforms.

Typically these files don't need to be updated often, but should probably be
ran about once a month to check for expirations. To do so, you can just type
`make certs` on a non Windows platform to update any dynamic bundles we're
using. Right now that just includes `mozilla.pem`.

