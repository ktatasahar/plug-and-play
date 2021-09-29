import React,{useState} from 'react'

import 'bootstrap/dist/css/bootstrap.min.css';

import { Container, Row ,Col} from 'react-bootstrap';

import Header from './compoment/Header';
import Temp from './compoment/Temp' 
import Humidity from './compoment/Humidity';
import Distance from './compoment/Distance';
import Dust from './compoment/Dust';
import Force from './compoment/Force';
import Gaz from './compoment/Gaz'; 

import {io} from "socket.io-client"



function App(props) {
  const [T, setT] = useState("");
  const [H, setH] = useState(""); 
  const [D, setD] = useState(""); 
  
  const socket = io('http://localhost:3001');
  React.useEffect(() => { socket.once('connect', () => {
    console.log('I m connected with the back-end');
    
    socket.on('disconnect', () => {
      console.log('user disconnected');
    });

    socket.on("message", data => {
      console.log(data); 
      if (data.d.temperature) setT(data.d.temperature) ;
      if (data.d.humid) setH(data.d.humid);
      if (data.d.dist) setD(data.d.dist)} 
    );
  }); });
  return (
    <div className="container">
        {D}
        <Header />
        <Container>
            <Row>
                <Col><Temp temprature={T} /></Col>
                <Col><Humidity humid={H}/></Col>
                <Col><Distance dist={D}/></Col>
           </Row>
           <Row>
                <Col><Force /></Col>
                <Col><Dust /></Col>
                <Col><Gaz /></Col>
            </Row>
        </Container>
        
        
        
        
        
        
       


    </div>
  );
}

export default App;
