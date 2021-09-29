import React from 'react';
import { Button } from 'react-bootstrap';
const Distance = (props) => {
    console.log(props) ;
    return (
        <div className="name"> 
           <h1> Distance </h1>  
           
            <Button variant="dark" >
                 <p>{props.dist} </p>
            </Button>{' '}
           
        
        
        </div>
    );
};

export default Distance;