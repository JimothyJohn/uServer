#include "Arduino.h"

// Add Bootstrap fluid container wrapper
String cssString = {R"=====(
<style>
</style>
)====="
};

String javaString = {R"=====(
<script>
</script>
)====="
};

String headerString = {R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
  <title>Microserver</title>
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.5.2/js/bootstrap.min.js"></script> 
)====="+cssString+javaString+"</head>"
};

String bodyString = {R"=====(
)====="};

String footerString = {R"=====(
</html>
<footer class="fixed-bottom">
      <div class="container-fluid">
          <div class="row">
            <div class="col text-center">
              <p style="font-size: 15px">Copyright © 2020 advin.io. All rights reserved.</p>
            </div>
          </div>
      </div>
</footer>
)====="};

String navHTML() {
  String ptr = headerString;
  ptr += bodyString;
  ptr += footerString;
  return ptr;
}
