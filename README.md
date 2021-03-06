# Simple 'XmlToJson' converter

## Overview

* xml to json by using boost::property_tree
* need xml decoration with "xmlattr"

## Xml Format (Draft)
* Give type information at xml tag
```
[ XML ]
<a type="integer">aaa</a>
<b type="string">bbb</b>
<c type="array:CCC">
  <CCC>
    ...
  </CCC>
  <CCC>
  </CCC>
</d>
<d type="sequence"> --> if omitted, default "sequence"
  <d1 type="string">ddd</d1>
  <d2 type="integer">ddd</d2>
</d> --> if omitted, default "sequence"

[ JSON ]
{
  "a": aaa,
  "b": "bbb",
  "c": [
    {
      ...
    },
    {
    }
  ],
  "d": {
    "d1": "ddd"
    "d2": ddd
  }
}
```
* Sample XML
```
<nrf-pdu>
    <validityPeriod type="integer">7</validityPeriod>
    <nfInstances type="array:NFProfile">
        <NFProfile>
            <nfInstanceId type="string">3fa85f64-5717-4562-b3fc-2c963f66afa6</nfInstanceId>
            <plmnList type="array:PlmnId">
                <PlmnId>
                    <mcc type="string">45000</mcc>
                    <mnc type="string">005</mnc>
                </PlmnId>
                <PlmnId>
                    <mcc type="string">45000</mcc>
                    <mnc type="string">006</mnc>
                </PlmnId>
            </plmnList>
            <perPlmnSnssaiList type="array:PlmnSnssai">
                <PlmnSnssai>
                    <plmnId>
                        <mcc type="string">45000</mcc>
                        <mnc type="string">005</mnc>
                    </plmnId>
                    <sNssaiList type="array:Snssai">
                        <Snssai>
                            <sst type="integer">0</sst>
                            <sd type="string">1234567890</sst>
                        </Snssai>
                        <Snssai>
                            <sst type="integer">1</sst>
                            <sd type="string">abcdefghij</sst>
                        </Snssai>
                    </sNssaiList>
                </PlmnSnssai>
            </perPlmnSnssaiList>
        </NFProfile>
        <NFProfile>
            <nfInstanceId type="string">3fa85f64-5717-4562-b3fc-2c963f66afa7</nfInstanceId>
            <plmnList type="array:PlmnId">
                <PlmnId>
                    <mcc type="string">45000</mcc>
                    <mnc type="string">005</mnc>
                </PlmnId>
            </plmnList>
            <perPlmnSnssaiList type="array:PlmnSnssai">
                <PlmnSnssai>
                    <plmnId>
                        <mcc type="string">45000</mcc>
                        <mnc type="string">005</mnc>
                    </plmnId>
                    <sNssaiList type="array:Snssai">
                        <Snssai>
                            <sst type="integer">0</sst>
                            <sd type="string">1234567890</sst>
                        </Snssai>
                        <Snssai>
                            <sst type="integer">1</sst>
                            <sd type="string">abcdefghij</sst>
                        </Snssai>
                    </sNssaiList>
                </PlmnSnssai>
            </perPlmnSnssaiList>
        </NFProfile>
    </nfInstances>
    <nrfSupportedFeatures type="string">80000000</nrfSupportedFeatures>
</nrf-pdu>
```
* Sample JSON
```
{
  "validityPeriod": 7,
  "nfInstances": [
    {
      "nfInstanceId": "3fa85f64-5717-4562-b3fc-2c963f66afa6",
      "plmnList": [
        {
          "mcc": "45000",
          "mnc": "005"
        },
        {
          "mcc": "45000",
          "mnc": "006"
        }
      ],
      "perPlmnSnssaiList": [
        {
          "plmnId": {
            "mcc": "45000",
            "mnc": "005"
          },
          "sNssaiList": [
            {
              "sst": 0,
              "sd": "1234567890"
            },
            {
              "sst": 1,
              "sd": "abcdefghij"
            }
          ]
        }
      ]
    },
    {
      "nfInstanceId": "3fa85f64-5717-4562-b3fc-2c963f66afa7",
      "plmnList": [
        {
          "mcc": "45000",
          "mnc": "005"
        }
      ],
      "perPlmnSnssaiList": [
        {
          "plmnId": {
            "mcc": "45000",
            "mnc": "005"
          },
          "sNssaiList": [
            {
              "sst": 0,
              "sd": "1234567890"
            },
            {
              "sst": 1,
              "sd": "abcdefghij"
            }
          ]
        }
      ]
    }
  ],
  "nrfSupportedFeatures": "80000000"
}
```
