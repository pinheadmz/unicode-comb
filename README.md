# Unicode Comb

## Recursively scan all files in a directory for insecure Unicode characters.

### Background

Re: "bidirectional override"

https://trojansource.codes/

https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-42574

https://blog.rust-lang.org/2021/11/01/cve-2021-42574.html

Re: "HANGUL FILLER"

https://certitude.consulting/blog/en/invisible-backdoor/

### Unsafe characters

```
\u202a-\u202e
\u2066-\u2069
\u3164
```

### Usage

Compile and run with root directory as argument:

(example)
```
gcc unicode-comb.c && ./a.out /Users/matthewzipkin/Desktop/work/hsd
```

### Output

Lots and lots of files, we even check everything in `node_modules`, finally:

```
...
dir: /Users/matthewzipkin/Desktop/work/hsd/lib/dns
 scanning: /Users/matthewzipkin/Desktop/work/hsd/lib/dns/key.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/lib/dns/resource.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/lib/dns/server.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/lib/dns/common.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/lib/dns/nsec.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/lib/pkg.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/.eslintrc.json
 scanning: /Users/matthewzipkin/Desktop/work/hsd/SECURITY.md
OK: Scanned 1336 files in 277 directories in 35.000000 seconds
```

### You gotta problem if...

Copy one of the illegal characters for example from here:

https://unicode-explorer.com/c/202E

Or you can try to copy it from this line of code:

```
// sneaky char between these two colons :‮:
```

... and paste it anywhere in your codebase. Run...

```
dir: /Users/matthewzipkin/Desktop/work/hsd/test/util
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/util/common.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/util/memwallet.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/util/node-context.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/util/migrations.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/http-test.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/path-test.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/node-http-test.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/auction-test.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/slidingwindow-test.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/chain-test.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/headers-test.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/block-test.js
 scanning: /Users/matthewzipkin/Desktop/work/hsd/test/wallet-rpc-test.js
  illegal character e280ae found in file /Users/matthewzipkin/Desktop/work/hsd/test/wallet-rpc-test.js at position 1932
Assertion failed: (0), function scan, file unicode-comb.c, line 63.
Abort trap: 6
```