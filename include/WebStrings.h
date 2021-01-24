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

String headString = {R"=====(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no, text/html, charset=utf-8">
    <title>Microserver</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.0.0-beta1/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-giJF6kkoqNQ00vy+HMDP7azOuL0xtbfIcaT9wjKHr8RbDVddVHyTfAAsrekwKmP1" crossorigin="anonymous">
</head>
)====="
};

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
    String bodyString = R"=====(
<body>
<nav class="navbar navbar-expand-md navbar-dark fixed-top bg-dark">
    <div class="container-fluid">
        <a class="navbar-brand" href="#">Fixed navbar</a>
        <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarCollapse" aria-controls="navbarCollapse" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbarCollapse">
            <ul class="navbar-nav me-auto mb-2 mb-md-0">
                <li class="nav-item active">
                <a class="nav-link" aria-current="page" href="#">Home</a>
                </li>
                <li class="nav-item">
                <a class="nav-link" href="#">Link</a>
                </li>
                <li class="nav-item">
                <a class="nav-link disabled" href="#" tabindex="-1" aria-disabled="true">Disabled</a>
                </li>
            </ul>
            <form class="d-flex">
                <input class="form-control me-2" type="search" placeholder="Search" aria-label="Search">
                <button class="btn btn-outline-success" type="submit">Search</button>
            </form>
        </div>
    </div>
</nav>
<main class="container">
    <div class="bg-light p-5 rounded">
        <h1>Navbar example</h1>
        <p class="lead">This example is a quick exercise to illustrate how fixed to top navbar works. As you scroll, it will remain fixed to the top of your browser’s viewport.</p>
        <a class="btn btn-lg btn-primary" href="#" role="button">View navbar docs »</a>
    </div>
</main>
</body>
)=====";
    return headString+bodyString+footerString;
}
