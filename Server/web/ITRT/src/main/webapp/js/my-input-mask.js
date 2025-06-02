/**
 * 
 */

 
 
 var ipv4_address = $('#ipAddress');
ipv4_address.inputmask({
    alias: "ip",
    numericInput:false,
    greedy: false //The initial mask shown will be "" instead of "-____".
});