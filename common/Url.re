type version =
  | Latest
  | NoVersion
  | Version(string);

/*
  Example 1:
  Url: "/docs/manual/latest/advanced/introduction"

  Results in:
  fullpath: ["docs", "manual", "latest", "advanced", "introduction"]
  base: ["docs", "manual"]
  version: Latest
  pagepath: ["advanced", "introduction"]
 */

/*
  Example 2:
  Url: "/apis/"

  Results in:
  fullpath: ["apis"]
  base: ["apis"]
  version: None
  pagepath: []
 */

/*
  Example 3:
  Url: "/apis/javascript/v7.1.1/node"

  Results in:
  fullpath: ["apis", "javascript", "v7.1.1", "node"]
  base: ["apis", "javascript"]
  version: Version("v7.1.1"),
  pagepath: ["node"]
 */

type t = {
  fullpath: array(string),
  base: array(string),
  version,
  pagepath: array(string),
};

let isVersion = str =>
  Js.String2.match(str, [%re "/latest|v\\d+(\\.\\d+)?(\\.\\d+)?/"])
  ->Belt.Option.isSome;

let parse = (route: string): t => {
  let fullpath =
    Js.String2.(route->split("/")->Belt.Array.keep(s => s !== ""));

  let (base, version, pagepath) =
    Belt.Array.reduce(
      fullpath,
      ([||], NoVersion, [||]),
      (acc, next) => {
        let (base, version, pagepath) = acc;

        if (version === NoVersion) {
          if (isVersion(next)) {
            let version =
              if (next === "latest") {
                Latest;
              } else {
                Version(next);
              };
            (base, version, pagepath);
          } else {
            let base = Belt.Array.concat(base, [|next|]);
            (base, version, pagepath);
          };
        } else {
          let pagepath = Belt.Array.concat(pagepath, [|next|]);

          (base, version, pagepath);
        };
      },
    );

  {fullpath, base, version, pagepath};
};