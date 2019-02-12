#!/bin/sh

#asciidoctor --out-file index.html main.adoc
asciidoctor-epub3 --doctype book main.adoc
