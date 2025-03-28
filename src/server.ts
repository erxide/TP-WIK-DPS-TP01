import {createServer, IncomingMessage, ServerResponse} from 'http';

const PORT = 8080;

const server = createServer((req: IncomingMessage, res: ServerResponse) => {
    if (req.method === "GET" && req.url === "/ping") {
        const headersJSON = JSON.stringify(req.headers, null, 2);

        res.writeHead(200, {
            "Content-Type": "application/json",
            "Content-Length": Buffer.byteLength(headersJSON).toString()
        });

        res.end(headersJSON);
    } else {
        const NotFound = JSON.stringify({ error : 404 });
        
        res.writeHead(404, {
            "Content-Type": "application/json",
            "Content-Length": Buffer.byteLength(NotFound).toString()
        });

        res.end(NotFound);
    }
})

server.listen(PORT, () => {
    console.log(`Server running at http://localhost:${PORT}`);
});