<script lang="ts">
    import { state } from "./lib/socket.svelte";
    import Calibrate from "./modes/Calibrate.svelte";
    import RemoteControl from "./modes/RemoteControl.svelte";
    let tab: string;
</script>

<main>
    <ul>
        <li onclick={() => (tab = "rc")} class:active={tab == "rc"}>
            Remote Control
        </li>
        <li
            onclick={() => (tab = "calibrate")}
            class:active={tab == "calibrate"}
        >
            Calibrate
        </li>
    </ul>
    <div class="content">
        {#if state.connected}
            {#if tab == "rc"}
                <RemoteControl />
            {:else if tab == "calibrate"}
                <Calibrate />
            {:else}
                <div class="text">
                    <h1>
                        Connected to robot!<br />✅🤖✅<br />Please select a
                        mode.
                    </h1>
                </div>
            {/if}
        {:else}
            <div class="text">
                <h1>Connecting to robot...<br />🛜🤖🛜</h1>
            </div>
        {/if}
    </div>
</main>

<style>
    main {
        width: 100%;
        height: 100%;
        background-color: #0a1128;
        font-family: sans-serif;
        display: flex;
        flex-direction: column;
    }

    ul {
        width: 100%;
        display: flex;
        flex-wrap: wrap;
        justify-content: center;
        list-style: none;
        gap: 10px;
        padding: 10px;
    }

    li {
        border: 2px solid #fff;
        color: #fff;
        padding: 10px 20px;
        border-radius: 30px;
        cursor: pointer;
    }

    .content {
        flex: 1;
    }

    .text {
        width: 100%;
        height: 100%;
        display: flex;
        align-items: center;
        justify-content: center;
        color: #fff;
        text-align: center;
    }

    .active {
        background-color: #fff;
        color: #000;
    }
</style>
