[@bs.val] external btoa: string => string = "window.btoa";
[@bs.val] external encodeURIComponent: string => string = "encodeURIComponent";
// [@bs.scope "window"] [@bs.val]

let convertNumberByIntl: (bool, int, int, float) => string = [%bs.raw
  {|
    function (useGrouping, minimumFractionDigits, maximumFractionDigits, value) {
      return new Intl.NumberFormat(window.navigator.languages[0], { style: 'decimal', useGrouping, minimumFractionDigits, maximumFractionDigits }).format(value);
    }
  |}
];

let copyContent: string => unit = [%bs.raw
  {|
    function (value) {
      const textField = document.createElement("textarea");
      textField.innerText = value;
      document.body.appendChild(textField);
      textField.select();
      document.execCommand("copy");
      textField.remove();
    }
  |}
];

let jsonConvertQuery: ConseiljsType.conseilQuery => string = [%bs.raw
  {|
    function (value) {
      return JSON.stringify(value)
    }
  |}
];