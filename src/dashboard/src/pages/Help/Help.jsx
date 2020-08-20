import React from 'react';
import Iframe from "react-iframe";

const Help = () => {
  const url = 'www.baidu.com'
  return (
    <Iframe url={url} width="100%" height="100%" />
  );
};

export default Help;
