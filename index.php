<?php
    $accessToken = "Your token from line bot API";
    $arrayHeader = array();
        $arrayHeader[] = "Content-Type: application/json";
        $arrayHeader[] = "Authorization: Bearer {$accessToken}";
    $content = file_get_contents('php://input');
    $arrayJson = json_decode($content, true);
    $message = $arrayJson['events'][0]['message']['text'];
    $arrayPostData['replyToken'] = $arrayJson['events'][0]['replyToken'];
    $arrayPostData['messages'][0]['type'] = "text";
    $replytoken = $arrayJson['events'][0]['replyToken'];
    $message .= ",";
    $message .= $replytoken;
            $topic = "Your ESP8266 ALIAS";
            pubMqtt($topic,$message);
function pubMqtt($topic,$msg){
    $appid= "MyApp/"; //enter NETPIE SESSION your appid and add one "/"
    $key = "MyKey"; //enter your NETPIE SESSION key
    $secret = "MySecret"; //enter your NETPIE SESSION key
    $Topic = "$topic"; 
      put("https://api.netpie.io/microgear/".$appid.$Topic."?retain&auth=".$key.":".$secret,$msg);
  }
function put($url,$tmsg){ 
    $ch = curl_init();
    curl_setopt( $ch, CURLOPT_URL, $url );
    curl_setopt($ch, CURLOPT_POSTFIELDS, $tmsg);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 10);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
    curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "PUT");
    $response = curl_exec($ch);
    curl_close($ch);
    echo $response . "\r\n";
    return $response;
}
?>
