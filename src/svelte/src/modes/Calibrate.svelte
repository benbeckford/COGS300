<script lang="ts">
    import { send } from "../lib/socket.svelte";

    let running = $state(false);
    let right = $state(255);
    let left = $state(255);

    function save() {
        send(0x11, left, right);
    }

    function run() {
        send(0xff, left, right);
        running = true;
    }

    function stop() {
        send(0x00);
        running = false;
    }

    send(0xca, 0xfe, 2);
</script>

<div class="container">
    <h2>Left motor: {left}</h2>
    <input type="range" min="0" max="255" bind:value={left} class="slider" />
    <h2>Right motor: {right}</h2>
    <input type="range" min="0" max="255" bind:value={right} class="slider" />
    {#if running}
        <button onclick={() => stop()}>Stop</button>
    {:else}
        <button onclick={() => run()}>Run</button>
    {/if}
    <button onclick={() => save()}>Save</button>
</div>

<style>
    .container {
        display: flex;
        align-items: center;
        justify-content: center;
        flex-direction: column;
        gap: 20px;
        height: 100%;
        color: #fff;
        padding: 20px;
    }

    .slider {
        width: 500px;
        max-width: 100%;
    }

    button {
        font-size: 18px;
        border: none;
        box-shadow: none;
        background-color: #fff;
        color: #000;
        border-radius: 30px;
        padding: 10px 20px;
    }
</style>
