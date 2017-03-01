# What is this?

This is a highly experimental (which is a really nice way of saying bug-ridden and poorly designed)
code playground I'm experimenting with c++ for a BaseWatsonService SDK.   I'm using the NLU api for
a sample because it's still in development.  

# Is there anything safe here?

Well, no.  But there is a relatively complete libcurl method.  

# Prerequisits

You should install [jq](https://stedolan.github.io/jq/). The output
of all the commands is json, and `jq` is a cli json parser/tool
that is to json what awk is to text.  


# Building

You'll need to install `json.hpp`.  If your on a Mac, you can run
`brew tap nlohmann/json` and `brew install nlohmann_json` to install.  Otherwise
you'll need to get it from https://github.com/nlohmann/json

You'll also need to have Boost installed.  Lastly, you'll need `cmake`.

```
$ git clone git@github.com:kognate/nlutooling.git
$ cd nlutooling
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./nlutooling -h
```

