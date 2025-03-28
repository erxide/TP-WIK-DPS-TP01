import {createServer, IncomingMessage, ServerResponse} from 'http';
import * as os from 'os';

const PORT = 8080;
const hostname = os.hostname();

const server = createServer((req: IncomingMessage, res: ServerResponse) => {
    if (req.method === "GET" && req.url === "/ping") {
        const headers = { ...req.headers, hostname };
        const headersJSON = JSON.stringify(headers, null, 2);

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