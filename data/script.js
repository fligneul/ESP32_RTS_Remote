$(document).ready(function () {
    $('.ssid-link').click(function () {
        $("#ssid").val($(this).text());
        $("#passwd").val("");
    });

    $("#refreshBtn").click(function () {
        window.location.reload();
    });

    $("#restartBtn").click(function () {
        window.location.href = "/restart"
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