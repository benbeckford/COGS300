<script lang="ts">
    import {
        draggable,
        axis,
        bounds,
        BoundsFrom,
        events,
        Compartment,
        position,
        type DragEventData,
    } from "@neodrag/svelte";
    import { Tween } from "svelte/motion";
    import { expoOut } from "svelte/easing";

    let right_dist = $state(0);
    let left_dist = $state(0);

    const socket = new WebSocket("ws://arduino.local:81");

    const posLeft = new Tween(
        { x: 0, y: 205 },
        { easing: expoOut, duration: 1200 },
    );

    const leftCompartment = Compartment.of(() =>
        position({ current: $state.snapshot(posLeft.current) }),
    );

    const posRight = new Tween(
        { x: 0, y: 205 },
        { easing: expoOut, duration: 1200 },
    );

    const rightCompartment = Compartment.of(() =>
        position({ current: $state.snapshot(posRight.current) }),
    );

    let timeoutLeft: number | null = null;

    const dragLeft = (e: DragEventData) => {
        posLeft.set({ x: e.offset.x, y: e.offset.y }, { duration: 0 });

        if (timeoutLeft != null) {
            clearTimeout(timeoutLeft);
        }

        timeoutLeft = setTimeout(() => {
            socket.send(
                new Uint8Array([
                    e.offset.y >= 205 ? 0xf : 0,
                    Math.round(
                        ((e.offset.y >= 205
                            ? e.offset.y - 205
                            : 205 - e.offset.y) /
                            205) *
                            255,
                    ),
                ]).buffer,
            );
        }, 50);
    };

    let timeoutRight: number | null = null;

    const dragRight = (e: DragEventData) => {
        posRight.set({ x: e.offset.x, y: e.offset.y }, { duration: 0 });

        if (timeoutRight != null) {
            clearTimeout(timeoutRight);
        }

        timeoutRight = setTimeout(() => {
            socket.send(
                new Uint8Array([
                    e.offset.y >= 205 ? 0xff : 0xf0,
                    Math.round(
                        ((e.offset.y >= 205
                            ? e.offset.y - 205
                            : 205 - e.offset.y) /
                            205) *
                            255,
                    ),
                ]).buffer,
            );
        }, 50);
    };

    const endDragLeft = (_) => {
        if (timeoutLeft != null) {
            clearTimeout(timeoutLeft);
        }

        posLeft.target = { x: 0, y: 205 };
        socket.send(new Uint8Array([0, 0]).buffer);
    };

    const endDragRight = (_) => {
        if (timeoutRight != null) {
            clearTimeout(timeoutRight);
        }

        posRight.target = { x: 0, y: 205 };
        socket.send(new Uint8Array([0xf0, 0]).buffer);
    };

    socket.onmessage = (event) => {
        if (event.data.startsWith("R")) {
            left_dist = event.data.substring(2);
        } else {
            right_dist = event.data.substring(2);
        }
        console.log(event.data);
    };
</script>

<main>
    <div class="column">
        <h1>L <span style="font-size: 32pt">({left_dist}cm)</span></h1>
        <div class="slider">
            <div
                {@attach draggable(() => [
                    leftCompartment,
                    bounds(BoundsFrom.parent()),
                    axis("y"),
                    events({
                        onDrag: dragLeft,
                        onDragEnd: endDragLeft,
                    }),
                ])}
                class="grip"
            ></div>
        </div>
    </div>
    <div class="column">
        <h1>R <span style="font-size: 32pt">({right_dist}cm)</span></h1>
        <div class="slider">
            <div
                {@attach draggable(() => [
                    rightCompartment,
                    bounds(BoundsFrom.parent()),
                    axis("y"),
                    events({
                        onDrag: dragRight,
                        onDragEnd: endDragRight,
                    }),
                ])}
                class="grip"
            ></div>
        </div>
    </div>
</main>

<style>
    main {
        width: 100%;
        height: 100%;
        display: flex;
    }

    .column {
        flex: 1;
        height: 100%;
        background-color: black;
    }

    h1 {
        font-family: sans-serif;
        font-size: 96pt;
        text-align: center;
        color: white;
    }

    .slider {
        height: 500px;
        width: 100px;
        border-radius: 50px;
        background-color: #a7a7a7;
        overflow: hidden;
        margin: auto;
        padding: 0 5px;
    }

    .grip {
        width: 90px;
        height: 90px;
        border-radius: 45px;
        background-color: blue;
        position: relative;
    }
</style>
