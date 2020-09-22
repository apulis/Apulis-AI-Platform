import React, { useState, useEffect } from 'react';
import Iframe from "react-iframe";
import axios from 'axios';

const Help: React.FC = () => {
  const [docsUrl, setDocsUrl] = useState('')

  const apiGetUrlDocs = async () => {
    const url = `/DocsUrl`;
    await axios.get(url)
      .then((res) => {
        if (res) {
          setDocsUrl(res.data.docsUrl + 'en/handbook/index.html')
        }
      })
  }

  useEffect(() => {
    apiGetUrlDocs();
  }, [])

  return (
    <div style={{ width: '100%', height: '100%', marginTop: "-25px" }}>
      {docsUrl && <Iframe url={docsUrl} width="100%" height="100%" />}
    </div>
  );
};

export default Help;