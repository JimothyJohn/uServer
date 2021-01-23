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
<body data-spy="scroll" data-target="#sidemenu" data-offset="40">
<div class="d-flex flex-column min-vh-100">
    <nav class="navbar navbar-expand-md sticky-to navbar-light bg-light">
        <a class="navbar-brand" href="#home" data-target="#home:not(.show)" data-toggle="collapse" data-parent="#page-content">Home</a>
        <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbar1">
            <span class="navbar-toggler-icon"></span>
        </button>
        <div class="collapse navbar-collapse" id="navbar1">
            <ul class="navbar-nav">
                <li class="nav-item active">
                    <a class="nav-link" href="#" data-target="#contact:not(.show)" data-toggle="collapse" data-parent="#page-content">Contact</a>
                </li>
                <li class="nav-item">
                    <a class="nav-link" href="#" data-target="#page2:not(.show)" data-toggle="collapse">Page 2</a>
                </li>
                <li class="nav-item">
                    <a class="nav-link" href="#" data-target="#page3:not(.show)" data-toggle="collapse">Page 3</a>
                </li>
            </ul>
            <ul class="navbar-nav ml-auto">
                <li class="nav-item active">
                    <a class="nav-link" href="#modalAbout" data-toggle="modal">About</a>
                </li>
            </ul>
        </div>
    </nav>
    <main id="page-content" class="container-fluid py-3 overflowauto">
        <div class="position-">
            <div id="home" class="collapse show" data-parent="#page-content">
                <div class="row h-100">
                    <aside class="col-md-3">
                        <div class="mt-4 mb-3 pt-4 sticky-top" id="sidemenu">
                            <div class="pt-1">
                                <ul class="nav nav-pills flex-md-column flex-row justify-content-between bg-light">
                                    <li class="nav-item"><a href="#sec1" class="nav-link active">Consistent</a></li>
                                    <li class="nav-item"><a href="#sec2" class="nav-link">Responsive</a></li>
                                    <li class="nav-item">
                                        <a href="#sec3" class="nav-link">Compatible</a>
                                        <ul class="nav flex-md-column small d-none d-md-flex">
                                            <li class="nav-item"><a href="#sec3a" class="nav-link">Element Styles</a></li>
                                            <li class="nav-item"><a href="#sec3b" class="nav-link">The Grid</a></li>
                                        </ul>
                                    </li>
                                    <li class="nav-item"><a href="#sec4" class="nav-link">Customization</a></li>
                                    <li class="nav-item"><a href="#sec5" class="nav-link">Themes</a></li>
                                    <li class="nav-item"><a href="#sec6" class="nav-link">JavaScript</a></li>
                                </ul>
                            </div>
                        </div>
                    </aside>
                    <main class="col py-5">
                        <div class="row">
                            <div class="col">
                                <ul class="nav nav-tabs small" role="tablist">
                                    <li class="nav-item"><a class="nav-link text-uppercase active" data-toggle="tab" href="#tab1" role="tab">Home</a></li>
                                    <li class="nav-item ml-auto"><a class="nav-link text-uppercase" data-toggle="tab" href="#tab2" role="tab">List</a></li>
                                    <li class="nav-item"><a class="nav-link text-uppercase" data-toggle="tab" href="#tab3" role="tab">Table</a></li>
                                </ul>
                                <div class="tab-content py-3">
                                    <div class="tab-pane active" id="tab1" role="tabpanel">
                                        <p class="lead text-center mb-3">Beautiful content starts here.</p>
                                        <div class="anchor" id="sec1"></div>
                                        <h4>Consistent</h4>
                                        <p>Foremost, Bootstrap provides a consistent, uniform appearance and design. With very little effort, Bootstrap can be used to transform a hodge-podge site into a professional, visually appealing design.</p>
                                        <div class="anchor" id="sec2"></div>
                                        <h4>Responsive</h4>
                                        <p>Sites created with Bootstrap are designed to look & function well on any device including desktops, laptops, tablets and smartphones. Bootstrap is "mobile-first" which means the design for smaller mobile devices is considered foremost.</p>
                                        <hr>
                                        <div class="anchor" id="sec3"></div>
                                        <h4>Compatible</h4>
                                        <p>Bootstrap is a mature, tested code-base that is entirely based on standards and cross-browser compliance. Sites that properly utilize what Bootstrap offers will also benefit from this compatibility. The CSS is the most essential part of Bootstrap. The Bootstrap CSS can be used alone, without the Bootstrap JavaScript components. Generally speaking, Bootstrap’s CSS provides two things…</p>
                                        <div class="pl-4">
                                            <div class="anchor" id="sec3a"></div>
                                            <h6>Element Styles</h6>
                                            <p> These are built-in styles for HTML elements such as Buttons, fonts, colors, badges, typography, progress bars, alerts, etc… Using any of these Bootstrap Components is very easy. It’s just a matter of referencing the appropriate Bootstrap CSS class in the HTML markup. </p>
                                            <br>
                                            <div class="anchor" id="sec3b"></div>
                                            <h6>The Bootstrap Grid</h6>
                                            <p>Layout helpers using “rows” and “columns”. The Bootstrap Grid System is used for layout, specifically Responsive Layouts. Understanding how it works is vital to understanding Bootstrap. The Grid is made up of groupings of Rows & Columns inside 1 or more Containers.</p>
                                        </div>
                                        <hr>
                                        <div class="anchor" id="sec4"></div>
                                        <h4>Customization</h4>
                                        <p>You may want to customize Bootstrap for several reasons. You may want to change some aspect of the look or styles such as colors, fonts, or borders. You may want to change some aspect of the responsive layout like grid breakpoints or gutter widths. Additionally, you may want to extend Bootstrap classes with new custom classes (ie; btn-custom).</p>
                                        <div class="anchor" id="sec5"></div>
                                        <h4>Creating Themes</h4>
                                        <p>Of course customization is important since not everyone wants that overly recognizable Bootstrap look. Using the methods (CSS or SASS) described it’s fairly easy to transform your Bootstrap 4 sites with a unique look. Once you understand the basic of customization you're on the way to creating custom Bootstrap themes.</p>
                                        <div class="anchor" id="sec6"></div>
                                        <h4>JavaScript</h4>
                                        <p>The Bootstrap documentation, has a very large “Components” section that includes most of the stylized CSS Components that I explained before. However, there’s overlap between the Bootstrap CSS and JavaScript Components. CSS is used to style Components, while JavaScript is used to make Components functional. Some Bootstrap Components only provide CSS-driven formatting and style so they don’t utilize JavaScript. These CSS-only Components were explored earlier in the Bootstrap CSS section: Badges, Breadcrumbs, Buttons, Cards, Form inputs, Jumbotron, Pagination.</p>
                                    </div>
                                    <div class="tab-pane" id="tab2" role="tabpanel">
                                        <div class="card card-outline-primary mb-3">
                                            <div class="card-body">
                                                <h3 class="card-title">Primary</h3>
                                                <p class="card-text">With supporting text below as a natural lead-in to additional content.</p>
                                                <a href="#" class="btn btn-outline-secondary">Outline</a>
                                            </div>
                                        </div>
                                        <div class="card card-outline-primary mb-3">
                                            <div class="card-body">
                                                <h3 class="card-title">Primary</h3>
                                                <p class="card-text">With supporting text below as a natural lead-in to additional content.</p>
                                                <a href="#" class="btn btn-outline-secondary">Outline</a>
                                            </div>
                                        </div>
                                        <div class="card card-outline-primary mb-3">
                                            <div class="card-body">
                                                <h3 class="card-title">Content</h3>
                                                <p class="card-text">This is example content only for demonstration purposes.</p>
                                                <a href="#" class="btn btn-outline-secondary">Outline</a>
                                            </div>
                                        </div>
                                        <div class="card card-outline-primary mb-3">
                                            <div class="card-body">
                                                <h3 class="card-title">List</h3>
                                                <p class="card-text">With supporting text below as a natural lead-in to additional content.</p>
                                                <a href="#" class="btn btn-outline-secondary">Outline</a>
                                            </div>
                                        </div>
                                        <div class="card card-outline-primary mb-3">
                                            <div class="card-body">
                                                <h3 class="card-title">Card</h3>
                                                <p class="card-text">With supporting text below as a natural lead-in to additional content.</p>
                                                <a href="#" class="btn btn-outline-secondary">Outline</a>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="tab-pane" id="tab3" role="tabpanel">
                                        <div class="table-responsive">
                                            <table class="table table-hover table-bordered w-auto">
                                                <thead>
                                                    <tr>
                                                        <th class="w-25">25% width</th>
                                                        <th class="w-25">25% width</th>
                                                        <th class="w-50">50% width</th>
                                                    </tr>
                                                </thead>
                                                <tbody>
                                                    <tr>
                                                        <td><input class="form-control" type="text"></td>
                                                        <td><input class="form-control" type="text"></td>
                                                        <td><input class="form-control" type="text"></td>
                                                    </tr>
                                                    <tr>
                                                        <td>Data</td>
                                                        <td>More data</td>
                                                        <td>123,203</td>
                                                    </tr>
                                                    <tr>
                                                        <td>Data</td>
                                                        <td>More data</td>
                                                        <td>123,203</td>
                                                    </tr>
                                                    <tr>
                                                        <td>Responsive</td>
                                                        <td>More data</td>
                                                        <td>53,203</td>
                                                    </tr>
                                                    <tr>
                                                        <td>Columns</td>
                                                        <td>More data</td>
                                                        <td>123,203</td>
                                                    </tr>
                                                    <tr>
                                                        <td>Grid</td>
                                                        <td>More data</td>
                                                        <td>12,303</td>
                                                    </tr>
                                                    <tr>
                                                        <td>Data</td>
                                                        <td>More text</td>
                                                        <td>73,233</td>
                                                    </tr>
                                                </tbody>
                                            </table>
                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </main>
                </div>
            </div>
            <div id="contact" class="collapse" data-parent="#page-content">
                <h1>Contact</h1>
                <p> Biodiesel taxidermy organic post-ironic, Intelligp v PBR narwhal sustainable mixtape swag wolf squid tote bag. Tote bag cronut semiotics, raw denim deep v taxidermy messenger bag. Tofu YOLO Etsy, direct trade ethical Odd Future jean shorts paleo. Forage Shoreditch tousled aesthetic irony, street art organic Bushwick artisan cliche semiotics ugh synth chillwave meditation. Shabby chic lomo plaid vinyl chambray Vice. Vice sustainable cardigan, Williamsburg master cleanse hella DIY 90's blog.</p>
            </div>
            <div id="page2" class="collapse" data-parent="#page-content">
                <h1>Page 2</h1> Artisan Wes Anderson wolf pug. Godard sustainable you probably haven't heard of them, vegan farm-to-table Williamsburg slow-carb readymade disrupt deep v. Meggings seitan Wes Anderson semiotics, cliche American Apparel whatever. Helvetica cray plaid, vegan brunch Banksy leggings +1 direct trade. Wayfarers codeply PBR selfies. Banh mi McSweeney's Shoreditch selfies, forage fingerstache food truck occupy YOLO Pitchfork fixie iPhone fanny pack art party Portland.
            </div>
            <div id="page3" class="collapse" data-parent="#page-content">
                <h1>Page 3</h1> Ethical Kickstarter PBR asymmetrical lo-fi. Dreamcatcher street art Carles, stumptown gluten-free Kickstarter artisan Wes Anderson wolf pug. Godard sustainable you probably haven't heard of them, vegan farm-to-table Williamsburg slow-carb readymade disrupt deep v. Meggings seitan Wes Anderson semiotics, cliche American Apparel whatever. Helvetica cray plaid, vegan brunch Banksy leggings +1 direct trade. Wayfarers codeply PBR selfies. Banh mi McSweeney's Shoreditch selfies, forage fingerstache food truck occupy YOLO Pitchfork fixie iPhone fanny pack art party Portland.
            </div>
        </div>
    </main>
</div>
</body>
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
