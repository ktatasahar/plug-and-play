
import mongoose from "mongoose"


const CONNECTION_URL = 'mongodb+srv://Plug_And_Play:280516@cluster0.ehgtd.mongodb.net/myFirstDatabase?retryWrites=true&w=majority'
const PORT = process.env.PORT || 5000 ; 

import Data from "./models/data.js";

import mqtt from 'mqtt'
import express from 'express'
import {request , response} from 'express'

import cors from 'cors'

import { createServer } from "http";
import { Server } from "socket.io"
const app = express() ; 


mongoose.connect(CONNECTION_URL, { useNewUrlParser:true, useUnifiedTopology:true})
.then(()=>app.listen(PORT, () => { console.log("connected to data base !!");

}
)) .catch((e)=>console.log("Can't connect to database",e))

const httpServer = createServer();
const io = new Server(httpServer, {
    cors: {
        origin: "*"
      }
});



//middlewares
app.use(express.json());
app.use(cors());
httpServer.listen(3001);

var options = {
    port: 1883, //don't put 8883 because it's encrypted
    host: 'mqtt://' + 'mqtt.flespi.io', //put your ibm cloud host in the empty string
    clientId: 'a:p9wc2t:' + Math.random().toString(16).substr(2, 8), // a:orgId: in the empty string
    username: 'FM6LtEMip1YaCSvUS24upMbw8uEmvqlY2gxkK9qXZeXrXh9F73XmFZGEh9OCpXof', // your API key
    keepalive: 60,
    reconnectPeriod: 1000,
    protocolId: 'MQIsdp',
    protocolVersion: 3,
    clean: true,
    encoding: 'utf8'};
   
    


var topicSub1="ESP32";
var client  = mqtt.connect(options.host, options)
console.log("connected flag "+client.connected);

io.once('connect', (socket) => {
  console.log('socket io connected');
  socket.emit('message', 'error');
  io.on('disconnect', (msg) => {
    console.log('io disconnected');
  });

});
client.on('message',function(topic,message,packet)
{
 message=JSON.parse(message) 
 if(message) Data.insertMany(message.d, function(err, res) {
  if (err) throw err;
  console.log("1 document inserted");
  })
 console.log(message);
 io.emit("message", message);


});
client.on("connect",function () {
  console.log("connected"+client.connected);
  
})
client.on('error',function (error) {
  console.log("Can't connect"+error);
  process.exit(1)
})
function publish(topicPub1,data,options)
 {
  console.log("publishing",data);
  if(client.connected==true){
    client.publish(topicPub1,data,options);
  }
}
function subscribe(topicSub,options)
{
 
  if(client.connected==true){
    client.subscribe(topicSub,options);
    console.log("subscribed successfully");
    
  }
}
client.subscribe(topicSub1,{qos:0});
app.get('/login',  (req, res) => {

  res.sendStatus(200);

})


