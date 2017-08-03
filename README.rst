Vmod-basicauth README
Copyright (C) 2013-2014 Sergey Poznyakoff
See the end of file for copying conditions.

* Overview

This module implements basic HTTP authentication against the password file
created with the htpasswd(1) utility.  The following password hashes are
supported: Apache MD5, crypt(3), SHA1, and plaintext.

* Example

import basicauth;

sub vcl_recv {
	if (!basicauth.match("/var/www/.htpasswd",
		             req.http.Authorization)) {
		error 401 "Authentication required";
	}
}	

* Installation

In order to compile the package you need to have Varnish source tree.
At least Varnish 3.0.1 is required.  Supposing that the varnish source tree
is available under /usr/src/varnish-3.0.1, run:

  ./configure --with-varnish-source=/usr/src/varnish-3.0.1

Once configured, do
  
  make

This will build the module.  After this step you can optionally run
'make test' to test the package.

Finally, do the following command as root:
  
  make install

* Build deb package

  dpkg-buildpackage

* Documentation

The manual page vmod_basicauth(3) will be available after successful
install.  To read it without installing the module, run
'man src/vmod_basicauth.3'.
  
* Bug reporting

Send bug reports and suggestions to <gray@gnu.org>


* Copyright information:

Copyright (C) 2013-2014 Sergey Poznyakoff

   Permission is granted to anyone to make or distribute verbatim copies
   of this document as received, in any medium, provided that the
   copyright notice and this permission notice are preserved,
   thus giving the recipient permission to redistribute in turn.

   Permission is granted to distribute modified versions
   of this document, or of portions of it,
   under the above conditions, provided also that they
   carry prominent notices stating who last changed them.

Local Variables:
mode: outline
paragraph-separate: "[ 	]*$"
version-control: never
End:
