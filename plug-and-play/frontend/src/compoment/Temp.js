import React from 'react';
import Thermometer from 'react-thermometer-component'
import './App.css';



const Temp = (props) => {
    console.log(props) ;
    return (
        <div className="name">
            <h1> Temperature </h1>
            <div className="thermometer">

                <Thermometer
                     
                     value={props.temprature}
                     max="100"
                     steps="2"
                     format="°C"
                     size="normal"
                     height="250"
                     
                     
                />
            </div>
        </div>
    );
};

export default Temp;