<script lang="ts">
    import { send } from "../lib/socket.svelte";

    let running = $state(false);
    let distance = $state(20);

    function start() {
        send(0x80 | distance);
        running = true;
    }

    function stop() {
        send(0x00);
        running = false;
    }

    send(0xca, 0xfe, 3);
</script>

<div class="container">
    <h3>Target distance from wall:</h3>
    <input
        type="number"
        min="0"
        max="127"
        class="number"
        bind:value={distance}
    />
    {#if running}
        <button onclick={() => stop()}>Stop</button>
    {:else}
        <button onclick={() => start()}>Run</button>
    {/if}
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

    button {
        font-size: 18px;
        border: none;
        box-shadow: none;
        background-color: #fff;
        color: #000;
        border-radius: 30px;
        padding: 10px 20px;
    }

    .number {
        font-size: 18px;
    }
</style>
