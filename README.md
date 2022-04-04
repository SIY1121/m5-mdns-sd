# M5 mDNS
M5Stick向けにdns/mdns/dns-sdの一部をフルスクラッチで実装したコード。

このプログラムを書き込んで本体を起動すると、
同一ネットワーク内のmdns対応ブラウザで http://m5.local/ にアクセスできる様になる。

DNSパケット処理部分は素のc++ 17でも動かせると思われる。

# 実装状況
### DNS https://datatracker.ietf.org/doc/html/rfc1035
パケット構造はrfc1035に準拠。
特定のAレコード問い合わせに対してレスポンスを返すだけのサーバー実装。

### MDNS https://datatracker.ietf.org/doc/html/rfc6762
マルチキャストアドレスをリッスンして普通のDNS応答を返すだけ。
mdns固有の仕様は実装していない。

### MDNS https://datatracker.ietf.org/doc/html/rfc6763
特定のサービスクエリに対して、PTRレスポンスを返すだけの実装。
