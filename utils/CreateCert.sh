#!/usr/bin/env bash

openssl ecparam -out contoso.key -name prime256v1 -genkey
openssl req -new -sha256 -key contoso.key -out contoso.csr
openssl x509 -req -sha256 -days 365 -in contoso.csr -signkey contoso.key -out contoso.crt
openssl ecparam -out fabrikam.key -name prime256v1 -genkey
openssl req -new -sha256 -key fabrikam.key -out fabrikam.csr
openssl x509 -req -in fabrikam.csr -CA  contoso.crt -CAkey contoso.key -CAcreateserial -out fabrikam.crt -days 365 -sha256
openssl x509 -in fabrikam.crt -text -noout
