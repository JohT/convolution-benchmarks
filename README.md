# Getting started with SIMD (Single Instruction Multiple Data)

This repository is intended to accompany the first steps in C++ with Single Instruction Multiple Data (SIMD).

## Compiler Explorer

- [Inner Loop Vectorization](https://godbolt.org/#z:OYLghAFBqd5TKALEBjA9gEwKYFFMCWALugE4A0BIEAZgQDbYB2AhgLbYgDkAjF%2BTXRMiAZVQtGIHgBYBQogFUAztgAKAD24AGfgCsp5eiyahUAUgBMAIUtXyKxqiIEh1ZpgDC6egFc2TEABWcncAGQImbAA5PwAjbFIQaVkAB3QlYhcmL19/IPI0jOchcMiYtnjE5PtsR2KmESIWUiIcvwDgh2wnLMbmolLouISk2SUmlra8zomBiKGKkeSASnt0H1JUTi5LAGYI1F8cAGozXY8AN26SUjPcMy0AQT2Do%2BxT89JjTHQ2O4fnk8iNg2CkjMCzh4iABPFLMdjvETsMHYAAqsOw/yeF3QBEwxwwTBxvnqAH0aD56PRSQBJJiRUihdDoFIANWuZAIAC8WPUIITxsdxpgQCArk4yJCkaDGOi4XdThYAGwRcgEoSC4Wi8U3KXI2UYhWWJUAawSkXoaq1Yo5t3O0pRcsxu1wx3WRGWALMAHYbE9jgHjiwfCR1UxBRERNz3mcACLHCIAOgyXOwEE9uz9j0DQZD6DDgrNpAtUdTH3jRYtyej6bOWZzwdDAqIbpDpZju3jkejiqsx0rtXbxwAtMceHWATnBKRjhBG/mCBAtBm%2BwQPh5W6Jo3Xe7YCJ7/YGffWc4H5wWW7p/OXZ2v/p3%2B%2BbBz3Rzxlh8AGIJkezgf0Ievu%2BIDHFoE6HqeuZNhql5sOoN4QHe5wJg0L5ju%2BZxfmuwHdmWr5gdmEHHNOs7nroEBXkwK7HLo65xtRsE7jRth0dIB4ERBx6ToRObumYgRWAQfHxsxD4IUwfFWLoQnHAAVI%2Bxa1BJa6jlJgSxuh1hcdx2mBmJElMdYY7SXJf5KT%2BBl9uOakaSeOk6Xp/EWccFjGfJFpmSpvbOUJNlaXZ3EOZJXm7K5pn8cp1HBT5%2BHcT6sZ%2BXFXrevFTwAgA9GlxzAMwCS8u8LDHF8TA/Gwxw6mQbo0EKPZMMcADuxBIGVEg%2BNgSjIac/HDjwapWfGDVEE11pFSVALAjKeWQjCcKsBwxyolijzWuVdoeAtLpZTlXzAuyEqkAA6o1ABK2ASOUlRKGJLZMGqqLHGwqrzfdLDqGxnHgUKRAijae2QutrorYdg1Hd8vznQkl03fN6Yrn5w2g2wpI4BcBBbIVCOxtgKNbDFgbWmwRA8AAnETuzeptTAQCNvyY9jabLLDH3Wj4TAENOiOkKd1KEOMpAELEIZZJCCo80QfMC3yD1Q2wL2M%2BxREVRAd3GhcLXvMBgPHQj4OkEobE5u98s5qrvgdvGovi4LQjQMwcsG8lfmc0QGy1ZrwPa8Muv4YlqVPBl80Ysc0LrDOGAnEgCTYGqFX0OgLD4sYxzYOo%2BrYImAIRC2hLElcV3HEwfjob6fnnhEKQhje2UMnlu03EDSAnWdnuXTwSq7AAHLII48ImWgfr1vcfnbZ55m5tSV1tNe2vXjf0DrLdKmqWiDwPffDwG57NpuQ50WXIbVqm6ZeX%2BB/0z%2B46ZnDX3arakI0LHvIKu6EDuu269hsSVtMOSlLUnSDJMhZLXTkPI%2BR7yIGqP80cQxyzilwVY9BuCBH4AELgOhyDoG4B4ZihklAhzRnsPg5AiDaHgasE0QQtCGG4NIFBpCMHcH4EoEAVCSFoPgeQOAsAUAYFBAwBIlBqC8JSPwxIhxjDAB4LsLQVC6D0GBLragsR6GxAiM0aE3AiG8I4MIAA8kwegGj2HkBwDLEwkhjGEE5j0K4zDjHJ26CGbYRDM61HofQfmXxSDQi8DgehYsCBsE0Rw%2B%2BLBgBKFZAQbAdVdEzWCXIYQYgJCcBkAkxQKgND0P0D1IwJg0A4LsB42IzDICrBZPUOxw5dG7BHMAEcNBVA0g3MOI6KQWBKCUHGWOLJhwrRfK09pShhwPQ6dgTAXTmQpF6baaMTDajXCyG4YqUwAg9TCPMHWBhCiZCECsrZ6QdlMEGJsnqXQehCD6JMbw7QDBnPqJcuYZRPa3NmHs05sxjnPLfGsDYWwDBi2wM4jhiCuDIPIKg9BmCuDqHbkqYcSppAElyXUqRvde6zmwZpawap8DEAqoQtUXg%2BGMBnIQ5Y/A2E6AZuQChgQqEgtoeQIJPAZHgvoVCphLDiGkOpSCiw/Agnem9ImXYJMO5aHbloaQRNAi7ECN6NlxiOXcvYasLhyAQDunLhAqg/JfgiJJVEBE3AYVwoRUiiRY5dhovQWMvFfMRQ9UEIk8QkhUnOvSWoTQxjsnkDql8FIwSEFILoUq7guiQzasqscU18LEXiJMFam1GL9WiMVLsN8FKeWrAjvHEY6ZqFcEZUEiwFhExKhkUqYmxM6XSARbsWQEL%2BDKuYaw7NhbdihshYwlVVLVhXF1os6QQA)

- [Own vectorizeable kernel centric convolution approach](https://gcc.godbolt.org/#z:OYLghAFBqd5TKALEBjA9gEwKYFFMCWALugE4A0BIEAZgQDbYB2AhgLbYgDkAjF%2BTXRMiAZVQtGIHgBYBQogFUAztgAKAD24AGfgCsp5eiyahUAUgBMAIUtXyKxqiIEh1ZpgDC6egFc2TEAsADnJ3ABkCJmwAOT8AI2xSEAA2AFZyAAd0JWIXJi9ff0CQrJznIQio2LYEpLT7bEdyphEiFlIiAr8A4IamvNb2okqY%2BMSU9KU2jq6i3qmhkeraiYBKe3QfUlROLksAZkjUXxwAajN9jwA3bCcyC9wzLQBBA6OT7HPLpQzjB6fXi8MEwpqcppgQCAbndSBcPDR6OgWEQHqdSMZMOg2CICAAvbD7CxfAAi5wA7DYXqdqTTqfsAHTSHgWLRaHg8fb7bAAWjZADFyADabTuWT6TwggBOaWE5IWHn8wVU4XUnj0smpLTSCxyyXy3k8AVClWnbmS%2BlBdkWVIWSU2hWGpXPE2nCz0rU25L7LT2g1G5Uq6T05JBaRk3WpVIO/3Ok3JcXJLQWfapaQ%2BhUWGMu1LirRpHiS6TJHjRp0u7lqsn7SWJllS0vGlXc%2BNkoJk9sa6Rcv1lk3ct2pZJd5OpqM9xvChkyIdJoL7ZINgOT9UWTvJSXexexlViwmajlthfjpe0%2BPJEOh1M8MeKicisWsqtaQn7IJb8tqlPryXX23vvsMl6ZLXjwwFvse27Cm6%2BztqyQQslo/4qjm4bzhY1roUhwpiskGo1vOaZYbSOZMmSdrFjIRE0jm1o8Oeq77CWEEum6P5FloGrWlR1L9oykapKBPrSNxpxqnqg7SsBTG3ieNIDqBVbAZqIluqytp4UyInNvSqRBGkr6Rt2MmQbSu5tmxaTgcZ5YMrp16pPOkpWZmvYqua%2BzaloTnJkexlmGSxIXJSgLPMCoLgpC0IkLClwIkiKL7LgpwAO4sDcjBEHyDBEIkXjYDQdCoAQzBEEoxIsD4CSoEgxVKHRJLnC8/nBdmjJMrp2oWF2rI9TwWi4LyLl3jSU7BEpkpeTIPVJv1zFxuqg7elWurftKE0DYqpwAPQAFSnAAsiw6gEGwfinCwcRKN4Pg5acVwSD42D0qcO1bcN1I5kEDmpiG86ptNfUbY672mmqum2n1NqRmta2zUmWZ9m6oZspG2o1gWMNw8DsnUkGZJdZeVYw5jG3SAjKpqmNYbaueGMk3NTaVo53p9TNANw0NOOmkGaZBPBDkbqz00c%2BTwqfk5fWrj%2BZLC6yG2cyZNJBOq7GFlqMvC5qG37KLIpBm2z42tKtrE4m2u6zS3IMtLaYwSbxPrbyZOucKLaptIaupA7dpw87IMVuKZJhrhyOy3LTt8gC/mBfswUAjlbAZEYOVwkQACeGTMOwnwAGoPdgAAqGfYP8LxXOgBCYKcADWiRRPQHglaQBCoF4TDl74zQQGFRBgkQEIgD8fyXD3px574hfF6iljJJEGQ3eQpyjxFg%2B/EwcKj%2BPj1F5n086rXpD14vy/95CQ/r5cW%2BT7viXnDqmxEPPRCrFHFITltW0HT49DOEnaenNgG4pB/5zxumCdgSdPjJWIEgU4RAkBQKQN4T4B9653VuNFc6TAq4sEwFXYgcD0BwIQacB%2BT96QTkEKQU4EAKokFOKAogABJbB2B1ANS0EFBhTAn4sJwOwuE3DyE5HxBAF%2Bsc742GsIwvhbCX5LmaiDKhNC6FENQY0WRAj9ikk4RI9R9BNFfA8DXOujR6QiOwGIrhthbD6M0fIxW1JFFc1pGQm6ZhUhWBkawgR1gTGHw0T4jxpIbHaKEe4zx3j%2BHBJev4%2BuHirB2KCakGOLUTTRwnBkpqAUo4vEiL3NgLBIhiMaq8N%2BS5R6qLvrPHhYCLiknRNgrEOJ8SEiChOSpN0iEz30Q1VK6VsCZWyrldA%2BVCrFWEGVCqVUarYDqskdpFShCgiqcCDuN08hhGYMAeBDVGHmLxJY1Yki4lmIsSU7kolFmQSqW43u9S%2B4DyivcWKiJkQPG7kIdZzQtkmHgYvLQ7o%2BTiLSdSfRjdhDN1bl866XcV7nwgIw9Yjyz5rwgPo5F8K0V3NWCC1%2BgUXhcHWPQbgqR%2BABC4Docg6BuAeBsX4q6Wwdh30YvwIg2giXrGriATUhhuDSHJRy6l3B%2BBKBAFocg7LKVEvIHAWAKAMCJwYIkSg1BFUZGVUkK4qAMgZAAPpXALHqwker1BDgEMM0gYqIBxCFXESI7Q07cD4OQRVHBhAAHkmD0CddK8gOBCkmEkH6wgpAMEEBuGKv1bDbg3V2C6/JjQhX0AIHEdEwCvA4CFUQZubBnUyoRCwYASgc7FWSh6zOFKXWCGEGICQnAZByGEMoNQmg/X6B4IYYwph6V2BTZdeA6x0AZGaFGraEVzDWFsCyba%2B1VDbQ9USLauAAASShUDbQAIrQm5GGrIHRAjbXaNVEAzwc4AA0LCisaOG1wEB3CzACJ28IkRRg1HGJ20ouQhCPoMF%2Bn5r7lgfr6Lelo0xOjXSKJ2hwoHBgdCWGMJI0HwO/uQ4sQDiGpDrEZdsBtkqw3xplSSrgZLyAUqpTSrgZrpCnDYEobVd0Cz0kJDQulU7rCL3wMQMgLLO2nC8Eqxg1CDg8FWGyjluLyDct5cRgVZGhWUdFeKyVEniXcCveQPNVZ6QSwLMydCqQZYOXk36xTKnpXrDlcgEAdzVWfME4kaI2duDUdo/Rjdhr3IaewIQaKlcDA1tEOISQjbAsto0EKjt5BkrogyPmtTJHBWme4B6m6T9SE0FOK5ujDHPPMaJBAATGqhO8bE%2BZnQkmEG4PGGIvlXA5N5p1O6CUQQtBtnbGjMMJmKMivsMpqVFWEv7CSz1rg4mLPrCATkVw0ggA%3D%3D)

- [Own vectorizeable kernel centric convolution approach (MSVC)](https://gcc.godbolt.org/#g:!((g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:79,endLineNumber:8,positionColumn:79,positionLineNumber:8,selectionStartColumn:79,selectionStartLineNumber:8,startColumn:79,startLineNumber:8),source:'%23include+%3Cvector%3E%0A%23include+%3Cspan%3E%0A%0Atemplate%3Ctypename+ValueType%3E%0Avoid+kernelCentricConvolution(const+std::span%3Cconst+ValueType%3E+%26input,+std::size_t+inputLength,+const+std::span%3Cconst+ValueType%3E+%26kernel,+std::size_t+kernelLength,+const+std::span%3CValueType%3E+%26output)+%7B%0A++++//+Multiply+every+input+sample+with+the+whole+kernel+vector+and+add+it+to+the+output.%0A++++for+(auto+inputIndex+%3D+0%3B+inputIndex+%3C+inputLength%3B+%2B%2BinputIndex)+%7B%0A++++++++for+(auto+kernelIndex+%3D+0%3B+kernelIndex+%3C+kernelLength%3B+%2B%2BkernelIndex)+%7B%0A++++++++++++output%5BinputIndex+%2B+kernelIndex%5D+%2B%3D+input%5BinputIndex%5D+*+kernel%5BkernelIndex%5D%3B%0A++++++++%7D%0A++++%7D%0A%7D%0A%0Aint+main()+%7B%0A++++const+auto+%26input+%3D+std::vector%3Cfloat%3E%7B0.34,+-0.71,+0.17,+-0.81,+0.20,+0.46,+0.61,+0.51,+-0.17,+-0.67,+-0.25,+0.31,+0.37,+0.73,+0.66,+-0.70%7D%3B%0A++++const+auto+%26kernel+%3D+std::vector%3Cfloat%3E%7B-0.23037,+0.7148,+-0.6308,+-0.0279,+0.1870,+0.0308,+-0.0328,+-0.0105%7D%3B%0A++++const+auto+convolutionLength+%3D+input.size()+%2B+kernel.size()+-+1%3B%0A++++auto+output+%3D+std::vector%3Cfloat%3E(convolutionLength,+0.0F)%3B%0A++++%0A++++kernelCentricConvolution(std::span(input),+input.size(),+std::span(kernel),+kernel.size(),+std::span(output))%3B%0A%7D%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0'),(g:!((h:compiler,i:(compiler:vcpp_v19_32_x64,filters:(b:'0',binary:'1',commentOnly:'0',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'/std:c%2B%2B20+/MP+/O2+/EHsc+/Qvec-report:2+/Qpar-report:2+/arch:AVX2',selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1,tree:'1'),l:'5',n:'0',o:'x64+msvc+v19.32+(C%2B%2B,+Editor+%231,+Compiler+%231)',t:'0')),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:73.98119122257053,n:'0',o:'',t:'0'),(g:!((h:output,i:(compilerName:'x64+msvc+v19.32',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x64+msvc+v19.32+(Compiler+%231)',t:'0')),header:(),l:'4',m:26.01880877742947,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)

- [Own vectorizeable kernel centric convolution approach (MSVC, pointer variation)](https://gcc.godbolt.org/#z:OYLghAFBqd5TKALEBjA9gEwKYFFMCWALugE4A0BIEAZgQDbYB2AhgLbYgDkAjF%2BTXRMiAZVQtGIHgBYBQogFUAztgAKAD24AGfgCsp5eiyahUAUgBMAIUtXyKxqiIEh1ZpgDC6egFc2TA3cAGQImbAA5PwAjbFIQAGZZAAd0JWIXJi9ffwMUtOchELDIthi4xPtsRwKmESIWUiIsvwCeSuqMuoaiIojo2IMlesbmnLah7t6SsqkASnt0H1JUTi5LeNDUXxwAajN4jwA3bCcyfdwzLQBBdc3t7D2DpSTjc8ub66JsNiSjL/2PEQAJ5JZjsB4ANQkPmwABUQdg3tdDugCJgdgBrWJhegeZhEUgEVBeJgo3w1CAYJhDHZDTAgEDPV4HKk0qG%2BOEI857CwANlCSR8RHItKI9MZBAAXtgAPpEHYCoVBZjAIhIEWs%2BV0hlMpgAzU7dkw%2BGg7mWXlY0g4kXaiXSuWY7FVZUmNUaoQ0226gFGzmm%2BK4Hm8xZEQVEWZ7ADsNmuOzjOwA9AmdgBZHz0Zy/IE7bDHUjZxVa9i/B4Ad2ISB2arLSG8D0tOJ2x1OpB2xnRLEw6OIVfQVaQDxDYYAdO94ztBK2ICwhX3CwBJJg4dSPAAiOy0%2BysCqYYcXy8eHh3YZdqqQW55NmsC6X2HUEbM0bH4/jk5209njqtVX3d7XG4vBsf1vFcAS/HFTzVC9bFsID6F/e8oxjK4X1QnYhyFMwAFYrBvA9bHA4Dl2w9cYPiddC2w3DdyFBCSJ2AAqQj6CouC6Kw1ct2fVDH042N41495BOud4vh%2BP5EQOYFQVYDhDWhP1JIuZFUXROC8WEQliSEMkhQyVRUWEWIlEpD15V9E1sCYwsbTFHUpVleVC0g9UdgNCyESYuDbPFNJ7XlOCXJFDzQSYjDwyQ7jE2TNMMwILMczzAsaKLcSywrfsazrZimxOEhW3bNsuwVeUSEy9ChRHKK3w/Mq8L/fZ103eJt3q0CDmPJUVSglrL1sNqHyffjUJqmcyrYkD/2a7cJvwjrAu689epg6xZrvQbkLQl9wqotrL2Y9jSOsRrOqIXaUsOxjmNYp14JAkiuOGl9hJQgTIz4m53qEkTrlCeU2BYUIIA2qKDTGvtzULf9bWbfKARoeh0BYM6A0fKwtGHCodgAWgxyM2g3YceEjEVceHAAOAmMYsLQRQx6ReTp4deSp4csIJsnidJjHeRJnHqawpn4lZ%2BI%2Bbx%2BImd5Rn%2BeHSNNy%2BlrQbMttP3NODobskBYbOA4EaRlGLmjMmLHiLRRaZ/HpHJ7nmdN62Za0CxIwATiZnhyblpmza0e2ybNixfYxrQeC0LDBMVp6wc/KldJqFz/0LYc/OwYH9rgpOHNT7Gdh4R7XrjcGKtDIUNfFbXSHhxHkfOUzSW8PTCkWr2ADFZjz8covU/EtJJWOMggL0XiYCBC3mU6M%2BlYGfJ1IeIDgsf0%2BTqfRV82fwtmNuI/z5iNIJIle/rmoDL%2B4yR5S4dMGRlhl8TpeF9ui%2Br%2BXxfM7H8LH/qYHN%2BQl6uHmehuBYX4AELgOhyDoG4B4Fa24lCLGWA8dYfByBEG0H/eYGIQBYVpgArg0hgGoPAdwfgSgQC0xQaAv%2B5A4CwBQBgH4DBYiUGoHQpIDC4iHFQEkJIMpDg8GdjKeIFgZTqF5LIOgGZjLUCiAQqIoQGhAm4EguhHBhAAHkmD0AURQ8gOAAYmEkNowgpA8oEGOCQ7Rd4ThClWEgk%2BOCwH0AIFEUg8ivA4AIXvNgijKEIxYMAJQEICDYFLKomS3i5DCDEBITgMgImKBUBoAh%2Bg2hGBMGgaBhgnEkMgPMdASQajmITNqcwx1rA00TCmVQiZVEWETLgAAEkoVAiYACKzZsbGJSI0EAtSEwtJeKQDp2AukoN6Q0VAKArgQgABoWGIVUExrgIDuFGK0cgwRQh9FKAMNoeR0hCFWbkVI%2BymBTH6HEcYCynCdGGE0euYx2iLNqLcs52yLn2FuYc8YLzNnTB2fMWBSwVgGAJNgGxlCcFAPICAsBECuAiOkDsNgSgOFNj4ZjWpEAoGlLsDsfAxAyA8mFiKLw9DGCtkQbMfg5CdAb3IBgrBhhuB4OhQQuFxDSHINQXSnBczyBeMjPEYcztKZ8J4BYCVWE5ZYQljC/g7KuUUPmNQ5AIBwpMNMmS2I4RwTcARUilFzTeHOwxfwbAhB8pogMIISJ4hJCxJtfEtQmhtHJPIKWFxSRvH/0Afg7RcLVGVRLugGgOx9XItRcajF75SWsPJUSngVLFW0vmAOTsAwp44JZV4vkw5g7k3Jj7SMxbpAWGdtIEmcrCFcA5WQ7lPquDxD9bCohya0HkDzGkVw0ggA%3D%3D)

- [Own vectorizeable kernel centric convolution approach with kernel index in outside loop(Clang)](https://gcc.godbolt.org/#z:OYLghAFBqd5TKALEBjA9gEwKYFFMCWALugE4A0BIEAZgQDbYB2AhgLbYgDkAjF%2BTXRMiAZVQtGIHgBYBQogFUAztgAKAD24AGfgCsp5eiyahUAUgBMAIUtXyKxqiIEh1ZpgDC6egFc2TEAtydwAZAiZsADk/ACNsUhAANgAOcgAHdCViFyYvX39A9MzsoTCI6LY4hJT7bEdnIREiFlIiPL8AoIdsJxymlqIyqNj4pNSlZtb2gq7JwfDhytGUgEp7dB9SVE4uSwBmcNRfHABqMz2PADceklJz3DMtAEF9w%2BPsM4ulNON7x5fnhgmBMThNMCAQNcnGRzh4aPR0CwiPcTqRjJh0GwRAQAF7YPYWT4AETOAHYbM8TlTqVS9gA6aQ8CxaLQ8Hh7PbYAC0rIAYuR/jSaVzSXSeMkAJxSgmJCzcvkCylCqk8OmkgCsWmkFllErlPJ4/MFypOXIldOSbIs6osEpt8sNiqeJpOFjpWptiT2WntBqNSuV0jpKWkpN16vVDv9zpNiTFiS0Fj26ukPvlFmjLvVYq0iXVPAl0kSPCjTpdXNVpL2EoTzMlpeNyq5cdJyVJ7Y10k5frLJq5bvViS7SZTkZ7jaF9JkQ8TyT2iQbAcnaosncSEu9i5jytFBM17LbC/HS5pccSKWS0hTPDHConwtFLKrWgJe2SW/LquT64lN9tH77ekvVJG8eBA99j23IU3T2dsWWSZktAA5VszDecLGtDDkKFUVEg1Gt51TbCaWzRlSTtYsZGI6ls2tHhz1XPYS0gl03V/IstA1a1qKpfsGQjfNSR9aQeJOVU9UHKUQOYu8T2pAcwKrEDNVEt0WVtfDGVE5s6XVZI8zfCNu1kqCaV3Nt2LzCCTPLek9JvdV5wlayM17ZVzT2bUtGcpMjxMsxSSJc4KQBJ4gRBMEIShW5YXhRFkT2XATgAdxYa5GCIXkGCIeIvGwGg6FQAhmCIJQiRYHw4lQJBiqUejiTOZ4ApCrMGUZPTtQsLsWR6ngtFwHlXPvakpwsNsbwlbyZB6xN%2BpY2M1UHb0q11H8pUmgaFROAB6AAqE4AFkWHUAg2D8E4WBiJRvB8HKTkuCQfGwOkTl27bhqpbNkkclMUnnFMZr6zbHQ%2B01VT020%2BptCN1vWubE0zPs3UvVkI21GsC1h%2BGQbkqkg1JLrL3VKtYaxzbpER5VVTG0lQ21c9MbJ%2Bam0rJzvT62bAfhobcdNINU2SBDHI3DmZu5ymhS/Zy%2BtXX8hLF8W3KFZI1Q4wstXlmbNU2vYJeFIM2xfG0pVtUmEx1vXqS5ek5dTWDTdJjaeQppXT0W6RpHV9VHbteGXdBisxVpsNW1TBWWXJ3l/gCoK9hC/4crYNIjBy2EiAATzSZh2A%2BAA1R7sAAFUz7A/meS50AITATgAa3iCJ6A8ErSAIVAvCYCvfAaJgIHCohQSIcEQG%2BX4Lj7k5898IuS5RSxEnCNJbvIE5x8i4efiYWFx8np7i6z2edTr0gG%2BX1fB4hEfN4uHfp/3xKzh1DYiEXogVmj8kJ227bDp8ehnGT9OJxsDXFIIAhet1QTsGTh8ZKxAkAnCIEgGBSBvAfCPg3e6NwyAXSYNXFgmBq7EAQegBBSCThPxfnSCcghSAnAgBVEgtd651AAJK4OwOoBqWhgpMOPqw9hnDYS8IbnSLIeIIBvzjg/Gw1h0H8JwOoFYZIWomhoXQhhJDwFEDYQorhPCtE6I4Z8DwJwtGiNxNgCRPDbC2AMQIpRzVQY0gobdMw6orB2N0bYYR8iOFuJJDYvYJItFuI8UwF%2Bhj1D%2BNej4%2BgoS5H0Eif44KoMY4TjSU1QK0dnjhH7mwFg4QJHKInOPDRD957hIgecEkaJcGYmxHiAkKSlylNuiQueCSGqpXStgTK2VcroHyoVYqwgyoVSqjVbAdVEjNKgq0xhQJO63RyCEZgwBEENTMWIyxDjrCxPMeIpRXIxKzJpGUlx/dqkDyHtFGEFw4pInuL3IQSzu6rJMIg5eWh3S8kkSoqkCSm7CBbm3F5N1u4QDXpfCAWi1jXIvhvCACS4VQsRRclYfz35BWeFwNY9BuDqn4AELgOhyDoG4B4GxezrqbG2A/Ji/AiDaFxWsGuIBNSGG4NIIlzKyXcH4EoEAWhyBMpJbi8gcBYAoAwEnBg8RKDUBlWkOVCQjjGGAOyFkAh%2BmkEFRAGIvKYjhBaOnbgfByAyo4MIAA8kwegpqxXkBwPkkwkhHWEFIDcAg1xBWOo4T0W6OxzW5LqLy%2BgBAYholAV4HAvKiAtzYGa8V8IWDACULnYqyVrVZ2JeawQwgxASE4DIOQwhlBqE0I6/QPBDDqrQFSuw4arrwDWOgNI3dfVci5GCap5hrC2GZKaAqLAJhcnyRsrkNBVAsJMVya1exTQACUfhKCUNUhEbauS3JbniJdK6lBjoIKu7AmB13oE3duixe6R0HuMBIdOWQ11BI3WkLdWCd0fEnUoNK3I23ODYLiJEOQuSeowKQauXJgAYHyEwLk4RBACrqF61wEB3DTACDW0ICwKhVAMBkLI3d0N4eKG87DIwEg1u6L0RocwiOUaQ9Rpg/RWhDBw6MSjtGboFA4wMVj5GpBrBpVsYtIrPVBvFfirghLyDEtJeSrg6h9LNmkCvIwJgxL0m%2BVoOhlL%2B3WGXvgYg2D9g1pOF4WVjBaEmZWIy5lGLyBso5ZJ7lMneXyYFUKkVdm8XcAsPwRNVY6TSwLEyDCxMfR7Fc469zXmxVrElcgEAFyFXPIs/ESIOduDqCHCcNgShLioHugWOkBJ%2BAnqMy3cENb82iHEJIEtNXy0aF5dW8gyU0RpCTT5qTPLovcGtbdF%2B5CaAnEU4kZTqn1UafdO6HTmJlWWfpTwGzsWdD2aQfg0YEjOVcBc4mnU7pxTJC0G2ds6NQxRbk/y%2BwnnRVre63sXrV2uC2bi2sEBWRXDSCAA
)
## References

- [Vectorization part1. Intro. (Denis Bakhvalov)](https://easyperf.net/blog/2017/10/24/Vectorization_part1)
- [Vectorization part2. Warmup. (Denis Bakhvalov)](https://easyperf.net/blog/2017/10/27/Vectorization_warmup)
- [Vectorization part3. Compiler report. (Denis Bakhvalov)](https://easyperf.net/blog/2017/10/30/Compiler-optimization-report)
- [Vectorization part4. Vectorization Width. (Denis Bakhvalov)](https://easyperf.net/blog/2017/11/02/Vectorization_width)
- [Vectorization part5. Multiversioning by data dependency. (Denis Bakhvalov)](https://easyperf.net/blog/2017/11/03/Multiversioning_by_DD)
- [Vectorization part6. Multiversioning by trip counts. (Denis Bakhvalov)](https://easyperf.net/blog/2017/11/09/Multiversioning_by_trip_counts)
- [Vectorization part7. Tips for writing vectorizable code. (Denis Bakhvalov)](https://easyperf.net/blog/2017/11/10/Tips_for_writing_vectorizable_code)
- [Microsoft Visual Studio Compiler - Vectorizer and parallelizer messages](https://docs.microsoft.com/en-us/cpp/error-messages/tool-errors/vectorizer-and-parallelizer-messages?view=msvc-170)