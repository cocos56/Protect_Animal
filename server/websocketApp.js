console.log("正在创建服务器")

var ws = require("nodejs-websocket")

{
    let port = 56
    var server = ws.createServer(function(conn){
        serverCallbackFunction(conn)
    }).listen(port)
}

function serverCallbackFunction(conn)
{
    console.log("A connection come")
    console.log("Sever connections = ", server.connections.length)
    //when a new message has been received.
    conn.on("text", function(str){ onMesage(str, conn) })

    //when a connection has been closed.
    conn.on("close", function(code, reason){ onClose(conn, code, reason) })

    //when a connection meet error.
    conn.on("error", function(code, reason){ onError(code, reason) })
}

function onMesage(msg, conn)
{
    msg = JSON.parse(msg)
    console.log("收到信息：", msg)
    msgBack = {}
    msgBack['msg'] = msg['msg']
    server.connections.forEach(function (conn) {
        conn.send(JSON.stringify(msgBack))
    })
}

function onClose(conn, code, reason)
{
    console.log("a connection close", code, reason)
    console.log("Sever connections = ", server.connections.length)
}

function onError(code, reason)
{
    console.log("a connection on error:", code , reason)
    console.log("Sever connections = ", server.connections.length)
}

console.log("创建服务器完毕")