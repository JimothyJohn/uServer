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
    "/About",
    "/IO.html",
    "/Variables.html",
    "/MQTT.html",
    "/Web.html",
  ];

  function RenderLink(link) {
    return(
      <a href={link.url} className={linkClass}>{link.name}</a>
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
        <div className={navClass}>
            {ListLinks(linkList)}
        </div>
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
    <BrowserRouter >
      <NavBar />
    </BrowserRouter>,
    document.getElementById('nav')
);
