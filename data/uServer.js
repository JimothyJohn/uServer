// uServer.js
const { BrowserRouter, Switch, Route, Link } = ReactRouterDOM;
const {
  Container,
  Row,
  Col,
  Navbar,
  Nav,
  ListGroup
} = ReactBootstrap;
const { useState } = React;

export default function NavBar(props) {

  return (
    <header>
      <Navbar collapseOnSelect bg="dark" variant="dark" expand="lg" className="shadow">
        <Container>
          <Navbar.Brand as={Link} to="/">µServer</Navbar.Brand>
          <Navbar.Toggle aria-controls="muh-toggle" />
          <Navbar.Collapse id="muh-toggle">
            <Nav className="me-auto">
              <Nav.Link as={Link} to="/io">Digital I/O</Nav.Link>
              <Nav.Link as={Link} to="/variables">Variables</Nav.Link>
              <Nav.Link as={Link} to="/mqtt">MQTT</Nav.Link>
              <Nav.Link as={Link} to="/files">File System</Nav.Link>
              <Nav.Link as={Link} to="/cloud">CloudAPI</Nav.Link>
            </Nav>
          </Navbar.Collapse>
        </Container>
      </Navbar>
    </header>
  );
}
