open ReasonUrql;
// Entry point
[@bs.val] external document: Js.t({..}) = "document";

let fetchOptions =
  Client.FetchOpts(Fetch.RequestInit.make(
    ~method_=Post,
    ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json", "Authorization": "Bearer XXX"}),
    (),
  ));

let client = Client.make(~url="https://api.github.com/graphql", ~fetchOptions, ());


// We're using raw DOM manipulations here, to avoid making you read
// ReasonReact when you might precisely be trying to learn it for the first
// time through the examples later.
let style = document##createElement("style");
document##head##appendChild(style);
style##innerHTML #= ExampleStyles.style;

let makeContainer = text => {
  let container = document##createElement("div");
  container##className #= "container";

  let title = document##createElement("div");
  title##className #= "containerTitle";
  title##innerText #= text;

  let content = document##createElement("div");
  content##className #= "containerContent";

  let () = container##appendChild(title);
  let () = container##appendChild(content);
  let () = document##body##appendChild(container);

  content;
};

ReactDOMRe.render(<Provider value=client><RacineMenu /></Provider>, makeContainer("Doggy"));
