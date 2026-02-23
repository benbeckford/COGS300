export const state = $state({ connected: false });

let socket: WebSocket;

export function send(...bytes: number[]) {
  socket.send(new Uint8Array(bytes).buffer);
}

export function receive(callback: (data: any) => any) {
  socket.onmessage = (event) => callback(event.data);
}

function connect() {
  socket = new WebSocket("ws://arduino.local:81");

  socket.onopen = () => (state.connected = true);

  socket.onclose = () => {
    state.connected = false;
    connect();
  };
}

connect();
