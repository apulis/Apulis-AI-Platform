import React from 'react';
import Iframe from "react-iframe";

const Help = () => {
  const url = 'http://10.31.3.230/docs'
  return (
    <div style={{width:'100%',height:'100%',marginTop:"-25px" }}>
    <Iframe url={url} width="100%" height="100%"/>
    </div>
  );
};

export default Help;