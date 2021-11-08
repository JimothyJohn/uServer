// Template.js

const pathname = window.location.pathname //returns the current url minus the domain name

function TrimPath(path) {
    return path.replace(/\//g, '').replace(/\.html/g, '');
}

function NavBar(props) {
  // Classes for each module
  const navClass = "navbar navbar-dark bg-dark shadow px-5";
  const linkClass = "navbar-brand px-3";
  const linkList = [
    "/Index.html",
    "/IO.html",
    "/Variables.html",
    "/MQTT.html",
    "/Web.html",
  ];

  function RenderLink(link) {
    return(
      <li><a href={link.url} className="dropdown-item">{link.name}</a></li>
      // <li><button className="dropdown-item" value={link.name}></button></li>
    );
  }

  // Automate navbar link creation
  function ListLinks(list) {
    const links = [];
    const link = {url: "", name: ""};
    for (var i=0; i<list.length; i++) {
      link.url = list[i].toLowerCase();
      link.name = TrimPath(list[i]);
      links.push(RenderLink(link));
    }
    return links;
  }

  return (
          <nav className="navbar navbar-expand-lg navbar-dark bg-dark shadow px-5">
            <div className="container-fluid">
              <div className="collapse navbar-collapse" id="navbarSupportedContent">

                <ul className="navbar-nav me-auto mb-2 mb-lg-0">
                  <li className="nav-item dropdown">
                    <a className="nav-link dropdown-toggle" href="#" id="navbarDropdown" role="button" data-bs-toggle="dropdown" aria-expanded="false">
                      Features
                    </a>
                    <ul className="dropdown-menu" aria-labelledby="navbarDropdown">
                      {ListLinks(linkList)}
                    </ul>
                  </li>
                </ul>
              </div>
              <p className="gray">Yo</p>
            </div>
          </nav>
  );
}

function Footer(props) {
    return(
        <div className="fixed-bottom container-fluid">
            <div className="row">
                <div className="col text-center">
                <a href="http://advin.io/"><img src="https://drive.google.com/uc?id=19gtVzQTrjAqPSP7G1lUsRx8mjidmgyCA" width="150px" /></a>
                <p>Copyright © 2020 advin.io. All rights reserved.</p>
                </div>
            </div>
        </div>
    )
}

ReactDOM.render(
    <Footer />,
    document.getElementById('footer')
)

ReactDOM.render(
    <NavBar />,
    document.getElementById('nav')
);
