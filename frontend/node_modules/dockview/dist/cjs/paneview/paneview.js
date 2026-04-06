"use strict";
var __assign = (this && this.__assign) || function () {
    __assign = Object.assign || function(t) {
        for (var s, i = 1, n = arguments.length; i < n; i++) {
            s = arguments[i];
            for (var p in s) if (Object.prototype.hasOwnProperty.call(s, p))
                t[p] = s[p];
        }
        return t;
    };
    return __assign.apply(this, arguments);
};
var __read = (this && this.__read) || function (o, n) {
    var m = typeof Symbol === "function" && o[Symbol.iterator];
    if (!m) return o;
    var i = m.call(o), r, ar = [], e;
    try {
        while ((n === void 0 || n-- > 0) && !(r = i.next()).done) ar.push(r.value);
    }
    catch (error) { e = { error: error }; }
    finally {
        try {
            if (r && !r.done && (m = i["return"])) m.call(i);
        }
        finally { if (e) throw e.error; }
    }
    return ar;
};
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.PaneviewReact = void 0;
var react_1 = __importDefault(require("react"));
var dockview_core_1 = require("dockview-core");
var react_2 = require("../react");
var view_1 = require("./view");
function extractCoreOptions(props) {
    var coreOptions = dockview_core_1.PROPERTY_KEYS_PANEVIEW.reduce(function (obj, key) {
        if (key in props) {
            obj[key] = props[key];
        }
        return obj;
    }, {});
    return coreOptions;
}
exports.PaneviewReact = react_1.default.forwardRef(function (props, ref) {
    var domRef = react_1.default.useRef(null);
    var paneviewRef = react_1.default.useRef();
    var _a = __read((0, react_2.usePortalsLifecycle)(), 2), portals = _a[0], addPortal = _a[1];
    react_1.default.useImperativeHandle(ref, function () { return domRef.current; }, []);
    var prevProps = react_1.default.useRef({});
    react_1.default.useEffect(function () {
        var changes = {};
        dockview_core_1.PROPERTY_KEYS_PANEVIEW.forEach(function (propKey) {
            var key = propKey;
            var propValue = props[key];
            if (key in props && propValue !== prevProps.current[key]) {
                changes[key] = propValue;
            }
        });
        if (paneviewRef.current) {
            paneviewRef.current.updateOptions(changes);
        }
        else {
            // not yet fully initialized
        }
        prevProps.current = props;
    }, dockview_core_1.PROPERTY_KEYS_PANEVIEW.map(function (key) { return props[key]; }));
    react_1.default.useEffect(function () {
        var _a;
        if (!domRef.current) {
            return function () {
                // noop
            };
        }
        var headerComponents = (_a = props.headerComponents) !== null && _a !== void 0 ? _a : {};
        var frameworkOptions = {
            createComponent: function (options) {
                return new view_1.PanePanelSection(options.id, props.components[options.name], { addPortal: addPortal });
            },
            createHeaderComponent: function (options) {
                return new view_1.PanePanelSection(options.id, headerComponents[options.name], { addPortal: addPortal });
            },
        };
        var api = (0, dockview_core_1.createPaneview)(domRef.current, __assign(__assign({}, extractCoreOptions(props)), frameworkOptions));
        var _b = domRef.current, clientWidth = _b.clientWidth, clientHeight = _b.clientHeight;
        api.layout(clientWidth, clientHeight);
        if (props.onReady) {
            props.onReady({ api: api });
        }
        paneviewRef.current = api;
        return function () {
            paneviewRef.current = undefined;
            api.dispose();
        };
    }, []);
    react_1.default.useEffect(function () {
        if (!paneviewRef.current) {
            return;
        }
        paneviewRef.current.updateOptions({
            createComponent: function (options) {
                return new view_1.PanePanelSection(options.id, props.components[options.name], { addPortal: addPortal });
            },
        });
    }, [props.components]);
    react_1.default.useEffect(function () {
        var _a;
        if (!paneviewRef.current) {
            return;
        }
        var headerComponents = (_a = props.headerComponents) !== null && _a !== void 0 ? _a : {};
        paneviewRef.current.updateOptions({
            createHeaderComponent: function (options) {
                return new view_1.PanePanelSection(options.id, headerComponents[options.name], { addPortal: addPortal });
            },
        });
    }, [props.headerComponents]);
    react_1.default.useEffect(function () {
        if (!paneviewRef.current) {
            return function () {
                // noop
            };
        }
        var disposable = paneviewRef.current.onDidDrop(function (event) {
            if (props.onDidDrop) {
                props.onDidDrop(event);
            }
        });
        return function () {
            disposable.dispose();
        };
    }, [props.onDidDrop]);
    return (react_1.default.createElement("div", { style: { height: '100%', width: '100%' }, ref: domRef }, portals));
});
exports.PaneviewReact.displayName = 'PaneviewComponent';
