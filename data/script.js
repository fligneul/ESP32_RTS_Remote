$(document).ready(function () {
    $('.ssid-link').click(function () {
        $("#ssid").val($(this).text().substring(0, $(this).text().lastIndexOf("(") - 1));
        $("#passwd").val("");
    });

    $("#refreshBtn").click(function () {
        window.location.reload();
    });

    $("#restartBtn").click(function () {
        if (confirm("Confirm reboot") == true) {
            window.location.href = "/restart";
        }
    });

    $("#mainMenuBtn").click(function () {
        window.location.href = "/";
    });

    $("#wifiMenuBtn").click(function () {
        window.location.href = "/wifi";
    });

    $("#mqttMenuBtn").click(function () {
        window.location.href = "/mqtt";
    });

    $("#otaMenuBtn").click(function () {
        window.location.href = "/ota";
    });

    $("#remotesMenuBtn").click(function () {
        window.location.href = "/remotes";
    });

    $("#createRemoteBtn").click(function () {
        window.location.href = "/remote";
    });
});