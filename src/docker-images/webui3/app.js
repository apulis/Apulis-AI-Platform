const Koa = require('koa');
const app = new Koa();

app.use(async (ctx) => {
    if (ctx.url === '/' && ctx.method === 'GET') {
        //显示表单页面
        let html = `
            <h2>koa2 request POST</h2>
            <form method="post" action="/">
            <p>userName</p> 
            <input name="userName">  <br>
            <p>age</p>
            <input name="age">
            <p>website</p>    
            <input name="website">    
            <button type="submit">submit</button> 
            </form>
        `;
        ctx.body = html;

    } else if (ctx.url === '/' && ctx.method === 'POST') {
        let postData = await parsePostData(ctx);
        ctx.body = postData;

    } else {
        ctx.body = '<h1>404<h1>'
    }

    function parsePostData() {
        return new Promise((resolve, reject) => {
            try {
                let postData = '';
                ctx.req.addListener('data', (data) => { // 有数据传入的时候
                    postData += data;
                });
                ctx.req.on('end', () => {
                    let parseData = parseQueryStr(postData);
                    resolve(parseData);
                });
            } catch (e) {
                reject(e);
            }
        })
    }

    // 处理 string => json
    function parseQueryStr(queryStr) {
        let queryData = {};
        let queryStrList = queryStr.split('&');
        console.log('queryStrList',queryStrList);
        console.log('queryStrList.entries()',queryStrList.entries());
        for(let [index,queryStr] of queryStrList.entries()){
            let itemList = queryStr.split('=');
            console.log('itemList',itemList);
            queryData[itemList[0]] = decodeURIComponent(itemList[1]);
        }
        return queryData;
    }
});
app.listen({{cnf["webuiport"]}}, () => {
    console.log('http://127.0.0.1:{{cnf["webuiport"]}}');
});