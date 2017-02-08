var cfg = `
event-handler configpush
   trigger on-startup-config
   action bash /mnt/flash/config_push.sh
!
transceiver qsfp default-mode 4x10G
!
logging buffered 1000
!
hostname {{hostname}}
ip name-server vrf default {{site}}
ip name-server vrf default {{site}}
ip name-server vrf default {{site}}
!
ntp server {{site}}
!
!
spanning-tree mode mstp
no spanning-tree vlan 4094
spanning-tree mst 0 priority 4096
!
aaa authentication login default group tacacs+ local
aaa authentication enable default group tacacs+ local
aaa authorization exec default group tacacs+ local
aaa authorization commands all default group tacacs+ local
aaa accounting commands all default start-stop group tacacs+
!
no aaa root
!
clock timezone America/New_York
!
vlan 1
   state suspend
!
{{role}}
!
interface Port-Channel1
!
interface Port-Channel4
!
interface Ethernet1
   load-interval 30
!
interface Ethernet2
   load-interval 30
!
interface Ethernet3
   load-interval 30
!
interface Management1
   ip address 10.0.5.19/24
!
ip route 0.0.0.0/0 {{site}}
!
no ip routing
{{role}}

{{model}}
!
management api http-commands
   no protocol https
   protocol unix-socket
   no shutdown
!
!
end
`;
var roleVals = {
  ds: 'DS STUFF!',
  tor: 'TOR STUFF!',
  htor: 'HTOR STUFF!',
  hcore: 'HCORE STUFF BRO!',
  svctor: 'SVCTOR LOL',
  stor: 'AHHHH STOR, MY MAN!',
};
var siteVals = {
  ami: 'AMI YEAH',
  dal: 'dal dal dal',
  'nj01': 'NJ01',
  nyi: 'NYI NYI',
  sje: 'S J E',
};
var modelVals = {
  '7050': '7050 stuff',
  '7150': '7150 stuff',
  '7048': '7048 stuff',
  '7500': '7500 stuff',
};

$(document).ready(function () {
  $('#config').val(cfg);
  $('#magic').on('submit', function (e) {
    e.preventDefault();
    var context = {
      hostname: $('[name=hostname]').val(),
      serialnumber: $('[name=serialnumber]').val(),
      ipaddress: $('[name=ipaddress]').val(),
      role: $('[name=role]').val(),
      site: $('[name=site]').val(),
      model: $('[name=model]').val(),
    };
    var replacedCfg = cfg.replace(/(\{{.*\}\})/g, function(v) {
      var newValue = v;
      if (v === '{{hostname}}') {
        newValue = context.hostname;
      } else if (v === '{{serialnumber}}') {
        newValue = context.serialnumber
      } else if (v === '{{ipaddress}}') {
        newValue = context.ipaddress
      } else if (v === '{{role}}') {
        newValue = roleVals[context.role];
      } else if (v === '{{site}}') {
        newValue = siteVals[context.site];
      } else if (v === '{{model}}') {
        newValue = modelVals[context.model];
      }
      return newValue;
    });
    $('#config').val(replacedCfg);
    // SEND AJAX REQ HERE
  });
});
