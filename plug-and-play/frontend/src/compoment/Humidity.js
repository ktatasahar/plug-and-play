import React from 'react';
import GaugeChart from 'react-gauge-chart' ;


const Humidity = (props) => {
    console.log(props) ;
    return (
        <div className="name">
        <h1> Humidity </h1>
        
        <div className="gauges">
           <GaugeChart 
             percent={props.humid}
             nrOfLevels={30} 
             max ="100"  
             min ="0"
             colors={["#9b217e", "#380f5e"]} 
             
                            
          
            />
        </div>
    </div>
    );
};

export default Humidity;